#include <arkanjo/base/function/function.hpp>

#include <arkanjo/formatter/format_manager.hpp>

const Path& Function::path() const {
    return path_;
}

Function::Function(const Path& _path)
    : path_(_path),
      location_{} {}

const FunctionLocation& Function::scope_location() const {
    return location_;
}

const std::vector<std::string>& Function::header() const {
    return header_;
}

std::vector<std::string> Function::build_all_content() const {
    std::vector<std::string> ret;
    // the last line of header should merge with the first line of content
    for (const auto& line : header_)
        ret.push_back(line);
    for (size_t i = 0; i < content_.size(); i++) {
        auto line = content_[i];
        if (i >= 1) {
            ret.push_back(line);
        } else {
            ret.back() += line;
        }
    }
    return ret;
}
