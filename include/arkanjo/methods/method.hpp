#pragma once

#include <vector>
#include <string>
#include <arkanjo/base/function/function_data.hpp>

/**
 * detected duplications
 * (similarity, path1, path2)
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

    virtual void execute(std::vector<FunctionData> functions) = 0;
};
