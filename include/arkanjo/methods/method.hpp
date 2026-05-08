#pragma once

#include <vector>
#include <arkanjo/base/function/function_data.hpp>

class IMethod {
public:
    virtual ~IMethod() = default;

    virtual void execute(std::vector<FunctionData> functions) = 0;
};
