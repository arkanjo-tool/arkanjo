/**
 * @file function.hpp
 * @brief Function abstraction for temporary codebase
 *
 * Represents a function in the temporary codebase with all related information.
 * Handles reading and managing function content, headers, and metadata.
 *
 * This is the abstraction of a function in the temporary codebase,
 * contains all the information related to a function, and for that
 * open and read the files in the temporary codebase.
 */

#pragma once

#include <array>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include <arkanjo/base/path.hpp>
#include <arkanjo/utils/utils.hpp>
using json = nlohmann::json;

/**
 * @brief Represents a code function with its content and metadata
 *
 * Provides access to function source, header, line information, and
 * various utility methods for function manipulation and display.
 */
class Function {
  private:
    static constexpr const char* LINE_DECLARATION_JSON = "line_declaration";   ///< JSON key for declaration line
    static constexpr const char* START_NUMBER_LINE_JSON = "start_number_line"; ///< JSON key for start line
    static constexpr const char* END_NUMBER_LINE_JSON = "end_number_line";     ///< JSON key for end line

    static constexpr const char* FUNCTION_PREFIX_PRINT = "Function Name: ";     ///< Prefix for function name display
    static constexpr const char* RELATIVE_PATH_PRINT = "Relative Path: ";       ///< Prefix for path display
    static constexpr const char* LINE_DECLARATION_PRINT = "Starts on line: ";   ///< Prefix for start line display
    static constexpr const char* END_DECLARATION_PRINT = "Ends on line: ";      ///< Prefix for end line display
    static constexpr const char* NUMBER_LINE_PRINT = "Total number of lines: "; ///< Prefix for line count display

    Path path;                        ///< Path object containing function location info
    std::vector<std::string> content; ///< Function source code content
    std::vector<std::string> header;  ///< Function header content
    int start_number_line;            ///< Line number where function starts
    int line_declaration;             ///< Line number of function declaration
    int end_number_line;              ///< Line number where function ends

    /**
     * @brief Reads the function's source content from file
     */
    void read_content();

    /**
     * @brief Reads the function's header content from file
     */
    void read_header();

    /**
     * @brief Reads the function's metadata from JSON file
     */
    void read_info();

  public:
    /**
     * @brief Calculates the total number of lines in the function
     * @return int Number of lines from declaration to end (inclusive)
     */
    int number_of_lines() const;

    /**
     * @brief Constructs a Function object
     * @param _path Path object containing function location info
     */
    explicit Function(const Path& _path);

    void load();

    /**
     * @brief Gets the function's line number scope
     * @return array<int,3> Array containing:
     *         [0] - Declaration line
     *         [1] - Start line
     *         [2] - End line
     */
    std::array<int, 3> get_scope_function_in_file() const;

    /**
     * @brief Gets the function's header content
     * @return vector<string> Each element represents a line of header
     */
    std::vector<std::string> get_header() const;

    /**
     * @brief Builds combined content of header and source
     * @return vector<string> Merged content with header and source
     */
    std::vector<std::string> build_all_content();

    /**
     * @brief Prints formatted function information to console
     */
    void print_basic_info();
};
