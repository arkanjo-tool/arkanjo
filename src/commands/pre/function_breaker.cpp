/*
This file intends to create a script to read a directory with
code in c and extract every function/struct of every .c file as a new file

Example of expected behaviour:
There is a file example.c with functions a and b, will be create two new
files: example/a.c and example/b.c

TODO: Doesn't work correct if the file has an incorrect bracket sequence,
even if the bracket sequence is in a commentary
*/

#include "function_breaker.hpp"

bool FunctionBreaker::is_c_extension(const std::string& extension) {
    for (auto c_extension : C_EXTENSIONS) {
        if (extension == c_extension) {
            return true;
        }
    }
    return false;
}

bool FunctionBreaker::is_java_extension(const std::string& extension) {
    for (auto java_extension : JAVA_EXTENSIONS) {
        if (extension == java_extension) {
            return true;
        }
    }
    return false;
}

bool FunctionBreaker::is_allowed_extension(const std::string& extension) {
    for (auto allowed_extension : ALLOWED_EXTENSIONS) {
        if (extension == allowed_extension) {
            return true;
        }
    }
    return false;
}

void FunctionBreaker::file_breaker(const fs::path& file_path, const fs::path& folder_path) {
    string extension = extract_extension(file_path);

    if (!is_allowed_extension(extension)) {
        return;
    }

    if (is_c_extension(extension)) {
        FunctionBreakerC function_breaker_c(file_path, folder_path);
    } else if (is_java_extension(extension)) {
        FunctionBreakerJava function_breaker_java(file_path, folder_path);
    } else {
        assert(false && "NOT ALLOWED FILE PASSED");
    }
}

void FunctionBreaker::function_breaker(const fs::path& folder_path) {
    for (const auto& dirEntry : fs::recursive_directory_iterator(folder_path)) {
        file_breaker(dirEntry.path(), folder_path);
    }
}

FunctionBreaker::FunctionBreaker(const fs::path& folder_path) {
    function_breaker(folder_path);
}
