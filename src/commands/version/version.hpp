/**
 * @file version.hpp
 * @brief Defines the version command interface.
 *
 * Provides the command used to display the current project version.
 */

#pragma once

#include <arkanjo/base/similarity_table.hpp>
#include <arkanjo/commands/command_base.hpp>

class Version : public CommandBase<Version> {
public:
    explicit Version(Similarity_Table* _similarity_table);

    COMMAND_DESCRIPTION("Version.");

    static constexpr CliOption options_[] = {
        OPTION_END
    };

    bool validate(const ParsedOptions& options) override;

    bool run(const ParsedOptions& options) override;
};
