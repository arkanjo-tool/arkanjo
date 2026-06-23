#pragma once

#include <arkanjo/cli/command_base.hpp>
#include <arkanjo/cli/parsed_options.hpp>

class PreprocessorClean : public CommandBase<PreprocessorClean> {
public:
    PreprocessorClean() = default;

    COMMAND_DESCRIPTION("Removes the application cache directory.")

    bool validate(const ParsedOptions& options) override;
    bool run(const ParsedOptions& options) override;
};
