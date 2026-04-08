/**
 * @file preprocessor_list.hpp
 * @brief Interface for listing preprocessors in the system
 *
 * Defines the PreprocessorList command that enumerates all available
 * preprocessors, providing metadata such as name, ID, disk usage, and
 * additional info. Useful for inspection and management before running
 * the main preprocessing pipeline.
 */

#pragma once

#include <arkanjo/commands/command_base.hpp>
namespace fs = std::filesystem;

/**
 * @brief Container metadata representation
 */
struct ContainerInfo {
    std::string name; ///< Container name
    std::string id; ///< Unique container ID
    std::string disk_usage; ///< Human-readable disk usage
    std::string extra; ///< Additional info
};

/**
 * @brief Lists all available preprocessor containers
 *
 * Handles user interaction for displaying containers, including optional
 * formatting without color. Provides validation and execution logic
 * for the command.
 */
class PreprocessorList : public CommandBase<PreprocessorList> {
private:
    bool no_color;

    /**
     * @brief Prints container information to the console
     * @param containers Vector of ContainerInfo objects to display
     */
    void print_containers(std::vector<ContainerInfo>& containers);

public:
    PreprocessorList();

    COMMAND_DESCRIPTION("Creates the foundation for fast query responses during the main operation phase.")

    bool validate(const ParsedOptions& options) override;

    bool run(const ParsedOptions& options) override;
};
