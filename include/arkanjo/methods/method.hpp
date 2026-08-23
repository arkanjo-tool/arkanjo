#pragma once

#include <arkanjo/base/function/function_data.hpp>
#include <arkanjo/base/similarity.hpp>
#include <string>
#include <vector>

/**
 * @brief Tuple format representing a detected duplication between two functions.
 * (similarity_percentage, path1, path2)
 */
using DuplicationEntry = std::tuple<Similarity, std::string, std::string>;

/**
 * @brief Interface for duplicate detection and analysis methods.
 */
class IMethod {
public:
    virtual ~IMethod() = default;

    /**
     * Output line format:
     *
     * path1 path2 similarity
     */
    virtual void save_duplications(std::vector<DuplicationEntry>&) {}

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
