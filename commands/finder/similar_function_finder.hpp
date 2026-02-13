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

#include "../base/similarity_table.hpp"
#include <arkanjo/base/function.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/utils/utils.hpp>

/**
 * @brief Similar function locator and reporter
 *
 * Identifies functions similar to a specified reference function pattern
 * and provides detailed reporting about the matches found.
 */
class Similar_Function_Finder {
    static constexpr const char* EMPTY_PATH_MESSAGE_1 = "There is no functions that resembles the name: ";                             ///< First part of no-results message
    static constexpr const char* EMPTY_PATH_MESSAGE_2 = "Value incorrected passed or there is no duplication code with this function"; ///< Second part of no-results message
    static constexpr const char* REFERENCE_PATH_MESSAGE = "The following function was found:";                                         ///< Reference function header
    static constexpr const char* COUNT_MESSAGE_1 = "The total number of functions that are similar to the found one is ";              ///< Similar count prefix
    static constexpr const char* COUNT_MESSAGE_2 = ". More info about them are listed below.";                                         ///< Similar count suffix

    Similarity_Table* similarity_table; ///< Source of similarity data
    std::string function_pattern;       ///< Function name pattern to match
    Path path;                          ///< Reference function path

    /**
     * @brief Locates path matching the function pattern
     */
    void find_path_that_meets_pattern();

    /**
     * @brief Prints message when no matching function found
     */
    void print_empty_path_message();

    /**
     * @brief Prints details about a single function
     * @param path Path of function to display
     */
    void print_function(Path path);

    /**
     * @brief Prints reference function information
     */
    void print_reference_path();

    /**
     * @brief Prints information about similar functions
     * @param similar_paths List of similar function paths
     */
    void print_similar_functions(std::vector<Path> similar_paths);

    /**
     * @brief Main function to print all similar functions
     */
    void print_similar_functions();

  public:
    /**
     * @brief Constructs finder and initiates search
     * @param _function_pattern Function name pattern to match
     * @param _similarity_table Source of similarity data
     */
    Similar_Function_Finder(std::string _function_pattern, Similarity_Table* _similarity_table);
};
