#include "help.hpp"

#include <iostream>

bool Help::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool Help::run([[maybe_unused]] const ParsedOptions& options) {
    std::cout << "Sorry I am tired I will do this latter" << '\n';

    return true;
}