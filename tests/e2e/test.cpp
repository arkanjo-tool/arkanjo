#include "../src/commands/pre/build/preprocessor_build.hpp"
#include <arkanjo/base/config.hpp>
#include <arkanjo/utils/utils.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

const fs::path EXPECTED_DIR = "tests/e2e/expected/tmp";
const fs::path CURRENT_DIR = "tests/e2e/current/tmp/arkanjo/default";
const fs::path CODEBASE_DIR = "tests/e2e/codebase";

std::vector<std::string> skip_check_list = {"config.txt", ".gitkeep"};

bool should_skip(std::string s) {
    for (auto x : skip_check_list) {
        if (x == s) {
            return true;
        }
    }
    return false;
}

// Special comparison for output_parsed.txt because the last column
// (similarity) is a floating-point value and is not guaranteed to be bitwise
// deterministic across platforms. We compare only structural fields and allow
// a small epsilon for the metric.
bool are_equal_output_parsed(const std::vector<std::string>& left, const std::vector<std::string>& right) {
    constexpr double EPS = 0.01;

    for (size_t i = 0; i < left.size(); ++i) {
        auto lp = left[i].find_last_of(' ');
        auto rp = right[i].find_last_of(' ');

        if (lp == std::string::npos || rp == std::string::npos) {
            if (left[i] == right[i])
                continue;

            return false;
        }

        if (left[i].substr(0, lp) != right[i].substr(0, rp))
            return false;

        double a = std::stod(left[i].substr(lp + 1));
        double b = std::stod(right[i].substr(rp + 1));

        double diff = std::abs(a - b);

        if (diff >= EPS)
            return false;

        if (diff > 0.0) {
            std::cerr << "similarity differs within EPS (ignored) at line "
                      << i + 1 << ": " << a << " vs " << b << "\n";
        }
    }

    return true;
}

bool are_equal_file(const fs::path& file1, const fs::path& file2) {
    auto left = Utils::read_file_with_vector(file1);
    auto right = Utils::read_file_with_vector(file2);

    if (left.size() != right.size())
        return false;

    const bool is_output_parsed = file1.filename() == "output_parsed.txt";

    if (is_output_parsed) {
        bool res = are_equal_output_parsed(left, right);
        return res;
    }
    
    return left == right;
}

std::map<fs::path, fs::path> collect(const fs::path& root) {
    std::map<fs::path, fs::path> files;

    for (const auto& entry : fs::recursive_directory_iterator(root)) {
        if (!entry.is_regular_file())
            continue;

        auto relative = fs::relative(entry.path(), root);

        if (should_skip(relative))
            continue;

        files.emplace(relative, entry.path());
    }

    return files;
}

bool Test() {
    auto expected_files = collect(EXPECTED_DIR);
    auto current_files = collect(CURRENT_DIR);

    for (const auto& [rel, expected_file] : expected_files) {
        auto it = current_files.find(rel);

        if (it == current_files.end()) {
            std::cout << "TEST FAILED" << '\n';
            std::cout << "Expected file with name " << expected_file << " but does not exist" << '\n';
            return false;
        }

        if (!are_equal_file(expected_file, it->second)) {
            std::cout << "TEST FAILED" << '\n';
            std::cout << "File " << expected_file << " does not have the expected content" << '\n';

            auto left = Utils::read_file_with_vector(expected_file);
            auto right = Utils::read_file_with_vector(it->second);

            for (size_t i = 0; i < std::min(left.size(), right.size()); ++i) {
                if (left[i] != right[i]) {
                    std::cout << "Difference at line " << i + 1 << '\n';
                    std::cout << "Expected: " << std::quoted(left[i]) << '\n';
                    std::cout << "Actual:   " << std::quoted(right[i]) << '\n';
                    break;
                }
            }

            if (left.size() != right.size()) {
                std::cout << "Different number of lines\n";
            }

            return false;
        }
    }

    for (const auto& [relative_path, _] : current_files) {
        if (expected_files.find(relative_path) == expected_files.end()) {
            std::cout << "TEST FAILED" << '\n';
            std::cout << "Unexpected file with name " << relative_path << " found" << '\n';
            return false;
        }
    }
    std::cout << "TEST PASSED" << '\n';

    return true;
}

bool is_project_root() {
    return fs::exists("build/arkanjo-test") &&
           fs::exists("tests") &&
           fs::exists("src");
}

int main(void) {
    if (!is_project_root()) {
        std::cerr
            << "Error: tests must be run from the project root.\n"
            << "Run:\n"
            << "    ./build/arkanjo-test\n";

        return EXIT_FAILURE;
    }

    Config::config().setTestConfig();

    PreprocessorBuild(true, CODEBASE_DIR, 0);

    return Test() 
        ? EXIT_SUCCESS
        : EXIT_FAILURE;
}
