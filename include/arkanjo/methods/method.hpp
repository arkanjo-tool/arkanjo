#pragma once

#include <vector>
#include <string>
#include <tuple>
#include <arkanjo/base/function/function_data.hpp>

/**
 * @brief Tuple format representing a detected duplication between two functions.
 * (similarity_percentage, path1, path2)
 */
using DuplicationEntry = std::tuple<double, std::string, std::string>;

/**
 * @brief Interface for duplicate detection and analysis methods.
 */
class IMethod {
public:
    virtual ~IMethod() = default;

    /**
     * @brief Exports detected duplication results.
     * @param duplications Output vector to populate with duplication entries.
     */
    virtual void save_duplications(std::vector<DuplicationEntry>& duplications) {
        (void)duplications;
    }

    /**
     * @brief Callback invoked for each processed function during codebase analysis.
     * @param fd Extracted function data and features.
     */
    virtual void on_function(const FunctionData& fd) = 0;

    /**
     * @brief Executes duplicate detection computation across all collected functions.
     */
    virtual void execute() = 0;
};
