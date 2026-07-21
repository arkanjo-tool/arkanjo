/**
 * @file function_loader.hpp
 * @brief Provides functionality to load Function objects from external sources.
 *
 * @see Function
 */

#pragma once

#include <arkanjo/base/function/function.hpp>
#include <arkanjo/base/path.hpp>

class FunctionLoader {
  public:
    /**
     * @brief Loads a function from a path.
     *
     * @param path Path identifying the function.
     * @return Loaded function if successful.
     */
    Function load(const Path& path);

  private:
    static constexpr const char* LINE_DECLARATION_JSON =
        "line_declaration"; ///< JSON key for declaration line
    static constexpr const char* START_NUMBER_LINE_JSON =
        "start_number_line"; ///< JSON key for start line
    static constexpr const char* END_NUMBER_LINE_JSON =
        "end_number_line"; ///< JSON key for end line

    /**
     * @brief Loads the function source content.
     *
     * Retrieves the source code associated with the function.
     */
    void read_content(Function& function);

    /**
     * @brief Loads the function header content.
     */
    void read_header(Function& function);

    /**
     * @brief Loads function metadata.
     *
     * Loads information such as source location and line ranges.
     */
    void read_info(Function& function);
};
