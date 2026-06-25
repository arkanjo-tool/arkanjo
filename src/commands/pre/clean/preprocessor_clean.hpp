/**
 * @file preprocessor_clean.hpp
 * @brief Remove cached application data.
 *
 * Defines the PreprocessorClean command, which deletes the application's
 * cache directory and all files stored within it.
 */

#pragma once

#include <arkanjo/commands/command_base.hpp>

class PreprocessorClean : public CommandBase<PreprocessorClean> {
public:
    PreprocessorClean() = default;

    COMMAND_DESCRIPTION("Removes the application cache directory.")

    bool validate(const ParsedOptions& options) override;
    bool run(const ParsedOptions& options) override;
};
