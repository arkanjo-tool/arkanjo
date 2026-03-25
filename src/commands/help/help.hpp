/**
 * @file random_selector.hpp
 * @brief Random selection of similar code pairs
 *
 * Provides functionality to randomly select and display pairs of similar
 * code segments within specified similarity ranges.
 *
 * The Random Selector random selects a set of duplicated functions pairs
 * enables to set the interval of acceptable similarity probability and
 * the number of functions to be selected.
 */

#pragma once

#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/commands/command_base.hpp>
#include <arkanjo/commands/commands_registry.hpp>

class Help : public CommandBase<Help> {
  private:
    const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>> internal_commands;
    const std::string command_name{""};

  public:
    COMMAND_DESCRIPTION("Help")

    explicit Help(
      const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands
    );

    explicit Help(
      const std::vector<std::pair<std::vector<std::string>, CommandsRegistry::CommandFactory>>& commands,
      const std::string command_name
    );

    bool validate(const ParsedOptions& options) override;

    /**
     * @brief Displays help information about available commands
     */
    bool run(const ParsedOptions& options) override;
};
