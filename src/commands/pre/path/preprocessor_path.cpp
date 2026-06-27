#include "preprocessor_path.hpp"
#include <arkanjo/base/config.hpp>

PreprocessorPath::PreprocessorPath() { }

bool PreprocessorPath::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool PreprocessorPath::run([[maybe_unused]] const ParsedOptions& options) {
    std::cout << Config::config().base_path.string() << "\n";
    return true;
}