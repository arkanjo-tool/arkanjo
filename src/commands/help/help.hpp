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

class Help : public CommandBase<Help> {
  public:
    COMMAND_DESCRIPTION("Help")

    bool validate(const ParsedOptions& options);

    /**
     * @brief Displays help information about available commands
     */
    bool run(const ParsedOptions& options) override;
};
