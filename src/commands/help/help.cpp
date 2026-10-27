#include "help.hpp"

#include <arkanjo/utils/os_utils.hpp>
#include <arkanjo/utils/utils.hpp>
#include <iostream>

Help::Help(
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands)
    : internal_commands(commands) {}

Help::Help(
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands,
    const std::string command_name_)
    : internal_commands(commands), command_name(command_name_) {}

bool Help::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool Help::run([[maybe_unused]] const ParsedOptions& options) {
    std::string title_str = Config::config().program_name;
    title_str += " ";
    if (!command_name.empty()) {
        title_str += command_name;
        title_str += " ";
    }

    std::ostringstream avaible_commands;

    for (const auto& [name, func] : internal_commands) {
        if (name.empty())
            continue;

        std::string external_cmd = Utils::external_command(Config::config().program_name, name[0]);
        bool exists = UtilsOSDependable::process_exists(external_cmd);

        if (func() == nullptr && !exists)
            continue;

        avaible_commands << "  " << name[0] << "\n";
    }
    if (avaible_commands.tellp() != std::streampos(0)) {
        std::cout << wrapped(title_str + " - Available commands:", 0);
        std::cout << "\n";
        std::cout << avaible_commands.str() << "\n";
    }

    if (options.has("all")) {
        std::ostringstream optional_commands;

        for (const auto& [name, func] : internal_commands) {
            std::string external_cmd =
                Utils::external_command(Config::config().program_name, name[0]);

            if (name.empty() || func() != nullptr)
                continue;
            if (UtilsOSDependable::process_exists(external_cmd))
                continue;

            optional_commands << "  " << name[0] << "\n";
        }
        if (optional_commands.tellp() != std::streampos(0)) {
            std::cout << wrapped("Optional commands not installed:", 0);
            std::cout << "\n";
            std::cout << optional_commands.str() << "\n";
        }
    }

    std::cout << wrapped(
        "Use '" + title_str + "<command> --help' for more information on a command.", 0
    );
    std::cout << wrapped("Use '" + title_str + "help --all' to list all known commands.", 0);

    return true;
}
