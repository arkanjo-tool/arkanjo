#include "help.hpp"

#include <iostream>

Help::Help(
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands)
    : internal_commands(commands) {}

Help::Help(
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands,
    const std::string command_name)
    : internal_commands(commands), command_name(command_name) {}

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
    title_str += " - Available commands:";
    std::cout << wrapped(title_str, 0);
    std::cout << "\n";

    for (const auto& [name, _] : internal_commands) {
        if (name.empty()) continue;
        std::cout << "  " << name[0] << "\n";
    }
    std::cout << "\n";

    std::string use_str = "Use '";
    use_str += Config::config().program_name;
    use_str += " ";
    if (!command_name.empty()) {
        use_str += command_name;
        use_str += " ";
    }
    use_str += "<command> --help' for more information on a command.";
    std::cout << wrapped(use_str, 0);

    return true;
}