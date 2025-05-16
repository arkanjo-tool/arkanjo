/**
 * @file function_breaker.hpp
 * @brief Main function extraction interface
 *
 * Provides the primary interface for processing source code directories
 * and extracting functions/structs into individual files in a temporary repository.
 * 
 * This file intends to create the process of reading a directory with 
 * code in c or java and extract every function/struct of every .c /.java file as a new file
 * in the temporary repository, aka, tmp/
 * 
 * Example of expected behaviour: 
 * There is a file example.c with functions a and b, will be create two new 
 * files: example/a.c and example/b.c
 * 
 * TODO: Doesn't work correct if the file has an incorrect bracket sequence, 
 * even if the bracket sequence is in a commentary
 */

#ifndef FUNCTION_BREAKER_HPP
#define FUNCTION_BREAKER_HPP

#include <bits/stdc++.h>
#include "../base/utils.hpp"
#include "function_breaker_c.hpp"
#include "function_breaker_java.hpp"
#include "function_breaker_util.hpp"
using namespace std;

/**
 * @brief Main function extraction processor
 *
 * Analyzes source code directories (C/C++/Java) and extracts each function
 * into individual files within a temporary repository. Handles file organization
 * and delegates language-specific parsing to appropriate handlers.
 *
 * @note Current limitations:
 * - May not work correctly with malformed bracket sequences
 * - May fail on brackets within comments
 * - Example behavior: For file example.c with functions a and b,
 *   creates example/a.c and example/b.c in tmp/ directory
 */
class FunctionBreaker {
        const vector<string> C_EXTENSIONS = { "c", "h" };          ///< Valid C/C++ file extensions
        const vector<string> JAVA_EXTENSIONS = { "java" };         ///< Valid Java file extensions
        const vector<string> ALLOWED_EXTENSIONS = { "c", "h", "java" };  ///< All supported extensions

        /**
         * @brief Checks if extension is for C/C++ files
         * @param extension File extension to check
         * @return bool True if extension is for C/C++
         */
        bool is_c_extension(string extension);

        /**
         * @brief Checks if extension is for Java files
         * @param extension File extension to check
         * @return bool True if extension is for Java
         */
        bool is_java_extension(string extension);

        /**
         * @brief Checks if extension is supported
         * @param extension File extension to check
         * @return bool True if extension is supported
         */
        bool is_allowed_extension(string extension);

        /**
         * @brief Processes individual source file
         * @param file_path Path to source file
         * @param folder_path Containing directory path
         */
        void file_breaker(string file_path, string folder_path);

        /**
         * @brief Processes all files in directory
         * @param folder_path Directory path to process
         */
        void function_breaker(string folder_path);

        public:
                /**
                 * @brief Constructs function breaker and processes directory
                 * @param folder_path Directory containing source files to process
                 */
                FunctionBreaker(string folder_path);
};

#endif