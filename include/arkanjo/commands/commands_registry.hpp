#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <arkanjo/commands/command.hpp>

/**
 * @brief Helpers for resolving CLI command names to command factories.
 */
namespace CommandsRegistry {
    /**
     * @brief Factory callback used to create a command instance.
     */
    using CommandFactory = std::function<std::unique_ptr<ICommand>()>;

    /**
     * @brief Builds a lookup map from every command alias to its factory.
     * @param commands List of aliases and factory callbacks.
     * @return Map where each alias points to the factory that creates the command.
     */
    inline std::unordered_map<std::string, CommandFactory> build_command_map(
        const std::vector<std::pair<std::vector<std::string>, CommandFactory>>& commands
    ) {
        std::unordered_map<std::string, CommandFactory> map;
        for (auto& cmd : commands) {
            for (auto& alias : cmd.first) {
                map[alias] = cmd.second;
            }
        }
        return map;
    }

    /**
     * @brief Creates the command registered for the requested name.
     * @param name Command name or alias received from the CLI.
     * @param commands List of aliases and factory callbacks.
     * @return Command instance when found, otherwise `nullptr`.
     */
    inline std::unique_ptr<ICommand> get_command(
        const std::string& name, 
        const std::vector<std::pair<std::vector<std::string>, CommandFactory>>& commands
    ) {
        static const auto internal_commands = build_command_map(commands);
        auto it = internal_commands.find(name);
        if (it != internal_commands.end()) {
            return it->second();
        }
        return nullptr; 
    }

} // namespace CommandsRegistry
