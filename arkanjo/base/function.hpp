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

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <bits/stdc++.h> 
// WARNING: This import may only work in Ubuntu
// See: https://github.com/stardust95/TinyCompiler/issues/2
#include <jsoncpp/json/json.h>

#include "path.hpp"
#include "utils.hpp"
using namespace std;

/**
 * @brief Represents a code function with its content and metadata
 *
 * Provides access to function source, header, line information, and
 * various utility methods for function manipulation and display.
 */
class Function {
        private:
                string LINE_DECLARATION_JSON = "line_declaration";    ///< JSON key for declaration line
                string START_NUMBER_LINE_JSON = "start_number_line";  ///< JSON key for start line
                string END_NUMBER_LINE_JSON = "end_number_line";      ///< JSON key for end line

                string FUNCTION_PREFIX_PRINT = "Function Name: ";    ///< Prefix for function name display
                string RELATIVE_PATH_PRINT = "Relative Path: ";       ///< Prefix for path display
                string LINE_DECLARATION_PRINT = "Starts on line: ";  ///< Prefix for start line display
                string END_DECLARATION_PRINT = "Ends on line: ";     ///< Prefix for end line display
                string NUMBER_LINE_PRINT = "Total number of lines: "; ///< Prefix for line count display

                Path path;                  ///< Path object containing function location info
                vector<string> content;     ///< Function source code content
                vector<string> header;      ///< Function header content
                int start_number_line;      ///< Line number where function starts
                int line_declaration;       ///< Line number of function declaration
                int end_number_line;        ///< Line number where function ends

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
                int number_of_lines();

                /**
                 * @brief Constructs a Function object
                 * @param _path Path object containing function location info
                 */
                Function(Path _path);

                /**
                 * @brief Gets the function's line number scope
                 * @return array<int,3> Array containing:
                 *         [0] - Declaration line
                 *         [1] - Start line
                 *         [2] - End line
                 */
                array<int,3> get_scope_function_in_file();

                /**
                 * @brief Gets the function's header content
                 * @return vector<string> Each element represents a line of header
                 */
                vector<string> get_header();

                /**
                 * @brief Builds combined content of header and source
                 * @return vector<string> Merged content with header and source
                 */
                vector<string> build_all_content();

                /**
                 * @brief Prints formatted function information to console
                 */
                void print_basic_info();
};

#endif