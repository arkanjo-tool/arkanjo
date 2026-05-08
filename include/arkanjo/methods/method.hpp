#pragma once

#include <vector>

class IMethod {
public:
    virtual ~IMethod() = default;

    virtual void execute() = 0;
};
