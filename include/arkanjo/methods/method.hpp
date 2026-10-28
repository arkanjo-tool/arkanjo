#pragma once

#include <arkanjo/base/function/function_data.hpp>
#include <arkanjo/base/similarity.hpp>
#include <string>
#include <vector>

/**
 * tuple format of the detected duplicates
 * (similarity_percentage, path1, path2)
 */
using DuplicationEntry = std::tuple<Similarity, std::string, std::string>;

class IMethod {
public:
    virtual ~IMethod() = default;

    /**
     * Output line format:
     *
     * path1 path2 similarity
     */
    virtual void save_duplications(std::vector<DuplicationEntry>&) {}

    virtual void on_function(const FunctionData& fd) = 0;

    virtual void execute() = 0;
};
