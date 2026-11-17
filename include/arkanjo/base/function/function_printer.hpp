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

    /**
     * @brief Formats path for display
     *
     * @param function Function to format.
     *
     * @return string Formatted path string
     */
    static std::string format_path_message_in_pair(const Function& function);

  private:
    static constexpr const char* kBetweenRelativeAndFunctionName =
        "::"; ///< Separator for path formatting
    static constexpr const char* kFunctionPrefixPrint =
        "Function: "; ///< Prefix for function name display
    static constexpr const char* kRelativePathPrint = "File: ";     ///< Prefix for path display
    static constexpr const char* kLineDeclarationPrint = "Lines: "; ///< Prefix for line display
    static constexpr const char* kNumberLinePrint = "Size: "; ///< Prefix for line count display
};
