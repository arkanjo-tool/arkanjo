#pragma once

#include <unordered_map>
#include <functional>
#include <memory>
#include <arkanjo/commands/command.hpp>

namespace CommandsRegistry {
    using CommandFactory = std::function<std::unique_ptr<ICommand>()>;

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