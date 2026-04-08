/**
 * @file similar_function_finder.hpp
 * @brief Similar function locator interface
 *
 * Provides functionality to find and display functions similar to
 * a given reference function within the codebase.
 *
 * Similar Functions Finder implements the functionality to given a function,
 * find the other functions that are similar to the given one.
 */

#pragma once

#include <string>
#include <vector>

#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/similarity_table.hpp>

#include <arkanjo/cli/cli_error.hpp>
#include <arkanjo/commands/command_base.hpp>

/**
 * @brief Similar function locator and reporter
 *
 * Identifies functions similar to a specified reference function pattern
 * and provides detailed reporting about the matches found.
 */
class SimilarFunctionFinder : public CommandBase<SimilarFunctionFinder> {
    static constexpr const char* TEMPLATE_EMPTY_FUNCTION =
      "There is no functions that resembles the "
      "name: {pattern:function}\n"
      "Value incorrected passed or there is no "
      "duplication code with this function";
    static constexpr const char* TEMPLATE_COUNT =
      "The total number of functions that are "
      "similar to the found one is {count}. "
      "More info about them are listed below.";
    static constexpr const char* REFERENCE_PATH_MESSAGE = "The following function was found:"; ///< Reference function header

    Similarity_Table* similarity_table; ///< Source of similarity data
    std::string function_pattern;       ///< Function name pattern to match
    bool open_folder;
    bool show_mode;
    bool no_numbers;
    Path path;                          ///< Reference function path

    /**
     * @brief Locates path matching the function pattern
     */
    void find_path_that_meets_pattern();

    /**
     * @brief Prints message when no matching function found
     */
    void print_empty_path_message() const;

    /**
     * @brief Prints details about a single function
     * @param path Path of function to display
     */
    Function get_reference_function(const Path& path) const;

    /**
     * @brief Prints reference function information
     */
    void print_reference_path();

    /**
     * @brief Prints information about similar functions
     * @param similar_paths List of similar function paths
     */
    void print_similar_functions(const std::vector<Path>& similar_paths);

    /**
     * @brief Main function to print all similar functions
     */
    void print_similar_functions();

    void handle_show_mode();

  public:
    static constexpr CliOption options_[] = {
        {"function_name", 0, PositionalArgument, "The tool will match the parameter with any function that has the `FUNCTION_NAME` as a substring."},
        {"open", 'o', NoArgument, "Open the folder where the function is located."},
        {"show", 's', NoArgument, "Show detailed information and the full source code of the function."},
        {"no-numbers", 0, NoArgument, "Hide line numbers in the displayed source code."},
        OPTION_END
    };
    COMMAND_DESCRIPTION("Search for functions using a substring match.")

    /**
     * @brief Constructs finder and initiates search
     * @param _function_pattern Function name pattern to match
     * @param _similarity_table Source of similarity data
     */
    explicit SimilarFunctionFinder(Similarity_Table* _similarity_table);

    bool validate(const ParsedOptions& options) override;

    /**
     * @brief Handles similar function finding command
     */
    bool run(const ParsedOptions& options) override;
};
