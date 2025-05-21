/**
 * @file path.hpp
 * @brief Path abstraction for temporary codebase
 *
 * Provides path manipulation utilities specific to the tool's directory structure.
 * Handles paths without requiring actual file system access.
 * Contains the abstraction of a path to a function in the temporary code base.
 * Useful for elements that does not need to, in fact, read the files in the 
 * temporary codebase.
 */

#ifndef PATH_HPP
#define PATH_HPP

#include <string>
#include <vector>
#include <cassert>
using namespace std;

/**
 * @brief Path manipulation class for tool-specific directory structure
 *
 * Manages paths to source files, headers, and metadata in the temporary codebase.
 * Provides methods for path construction and analysis without file I/O.
 */
class Path {
        private:
                string JSON_EXTENSION = ".json";      ///< Extension for JSON metadata files
                string BASE_INIT_STRING = "tmp";     ///< Base directory name
                string SOURCE_STRING = "source";     ///< Source subdirectory name
                string HEADER_STRING = "header";     ///< Header subdirectory name
                string INFO_STRING = "info";         ///< Info subdirectory name
                char BAR = '/';                      ///< Path separator character

                vector<string> tokens;               ///< Path components
                int position_start_relative_path;    ///< Index where relative path begins

                /**
                 * @brief Splits path string into components
                 * @param string_path Path to split
                 * @return vector<string> Path components
                 */
                vector<string> split_path(string string_path);

                /**
                 * @brief Finds start position of relative path
                 * @return int Index in tokens where relative path begins
                 */
                int find_position_start_relative_path();

                /**
                 * @brief Joins path components into string
                 * @param path Components to join
                 * @return string Constructed path
                 */
                string build_string_path(vector<string> path);

                /**
                 * @brief Builds path with specified base directory
                 * @param base Base directory to use
                 * @return string Constructed path
                 */
                string build_base_path(string base);

                /**
                 * @brief Extracts relative path components
                 * @return vector<string> Tokens for relative path
                 */
                vector<string> get_tokens_from_relative_path();

                /**
                 * @brief Removes file extension
                 * @param token String with extension
                 * @return String without extension
                 */
                string remove_extension(string token);

        public:
                /**
                 * @brief Checks if path is empty
                 * @return bool True if no path components
                 */
                bool is_empty();

                /**
                 * @brief Default constructor
                 */
                Path();

                /**
                 * @brief Constructs from path string
                 * @param string_path Path to parse
                 */
                Path(string string_path);

                /**
                 * @brief Builds source file path
                 * @return string Path to source file
                 */
                string build_source_path();

                /**
                 * @brief Builds header file path
                 * @return string Path to header file
                 */
                string build_header_path();

                /**
                 * @brief Builds metadata file path
                 * @return string Path to JSON metadata file
                 */
                string build_info_path();

                /**
                 * @brief Builds relative path portion
                 * @return string Relative path
                 */
                string build_relative_path();

                /**
                 * @brief Extracts function name from path
                 * @return string Function name without extension
                 */
                string build_function_name();

                /**
                 * @brief Finds common folders with another path
                 * @param path Other path to compare
                 * @return vector<string> Common base folders
                 */
                vector<string> get_common_folders(Path path);

                /**
                 * @brief Checks for pattern in path
                 * @param pattern Pattern to search for
                 * @return bool True if pattern found
                 */
                bool contains_given_pattern(string pattern);

                /**
                 * @brief Path comparison operator
                 * @param path Other path to compare
                 * @return bool True if this path is ordered first
                 */
                bool operator<(const Path &path) const;
};

#endif