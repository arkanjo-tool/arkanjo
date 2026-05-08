#pragma once

#include <string>

struct MetadataFeature {
    /**
     * @brief Full function signature.
     *
     * Includes return type, name, parameters, and qualifiers
     * (e.g., "int foo(int a, float b) const").
     */
    std::string signature;

    /**
     * @brief Line number where the function is declared.
     *
     * Typically corresponds to the line where the function signature begins.
     */
    size_t line_declaration;

    /**
     * @brief Starting line number of the function body.
     *
     * May differ from declaration line if attributes or macros are present.
     */
    size_t start_number_line;

    /**
     * @brief Ending line number of the function.
     *
     * Corresponds to the last line of the function body.
     */
    size_t end_number_line;
};
