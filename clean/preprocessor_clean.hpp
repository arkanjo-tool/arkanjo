#pragma once

#include <arkanjo/commands/command.hpp>

class PreprocessorClean : public ICommand {
  public:
    PreprocessorClean() = default;
    ~PreprocessorClean() override = default;

    bool validate(const ParsedOptions& options) override;
    bool run(const ParsedOptions& options) override;
};