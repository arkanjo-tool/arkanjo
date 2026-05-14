#pragma once

#include <vector>
#include <string>
#include <arkanjo/base/function/function_data.hpp>

/**
 * tuple format of the detected duplicates
 * (similarity_percentage, path1, path2)
 */
using DuplicationEntry = std::tuple<double, std::string, std::string>;

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
