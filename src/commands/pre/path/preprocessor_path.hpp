/**
 * @file preprocessor_path.hpp
 * @brief Interface for displaying the cache directory path
 * 
 * Defines the PreprocessorPath command that outputs the absolute path
 * of the ArKanjo cache directory. This is useful for manual maintenance,
 * inspecting cache contents, and troubleshooting.
 */

 #pragma once

 #include <arkanjo/commands/command_base.hpp>
 #include <iostream>

 /**
  * @brief Displays the location of the ArKanjo cache directory
  * 
  * Handles the execution of the 'path' command, which retrieves and
  * cleanly prints the base cache path configuration to standard output.
  */
class PreprocessorPath : public CommandBase<PreprocessorPath> {
public:
    PreprocessorPath();

    COMMAND_DESCRIPTION("Display the location of the ArKanjo cache directory.")

    bool validate(const ParsedOptions& options) override;

    bool run(const ParsedOptions& options) override;
};