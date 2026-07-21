/**
 * @file function_printer.hpp
 * @brief Provides utilities for printing function information and source code.
 *
 * @see Function
 */

#pragma once

#include <arkanjo/base/function/function.hpp>

class FunctionPrinter {
  public:
    /**
     * @brief Prints formatted function information to console
     *
     * @param function Function to print.
     */
    static void print_basic_info(const Function& function);

    /**
     * @brief Prints the function source code.
     *
     * @param function Function whose content will be printed.
     * @param no_numbers If true, line numbers are omitted.
     */
    static void print_code(const Function& function, bool no_numbers = false);

  private:
    static constexpr const char* FUNCTION_PREFIX_PRINT =
        "Function: "; ///< Prefix for function name display
    static constexpr const char* RELATIVE_PATH_PRINT = "File: ";     ///< Prefix for path display
    static constexpr const char* LINE_DECLARATION_PRINT = "Lines: "; ///< Prefix for line display
    static constexpr const char* NUMBER_LINE_PRINT = "Size: "; ///< Prefix for line count display
};
