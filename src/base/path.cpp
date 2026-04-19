
#include <arkanjo/base/path.hpp>

#include <iostream>
#include <arkanjo/base/config.hpp>
#include <arkanjo/commands/pre/preprocessor.hpp>

Path::Path(const fs::path& _path) {
    resource_path = _path;
}

fs::path Path::build_source_path() const {
    auto& cfg = Config::config();
    return cfg.base_path / cfg.name_container / cfg.source_path / resource_path;
}

bool Path::is_empty() const {
    return resource_path.empty();
}

fs::path Path::build_header_path() const {
    auto& cfg = Config::config();
    return cfg.base_path / cfg.name_container / cfg.header_path / resource_path;
}

fs::path Path::build_info_path() const {
    auto& cfg = Config::config();
    fs::path p = cfg.base_path / cfg.name_container / cfg.info_path / resource_path;
    p.replace_extension(JSON_EXTENSION);
    return p;
}

fs::path Path::build_relative_path() const {
    return resource_path.parent_path();
}

std::string Path::build_function_name() const {
    std::string filename = resource_path.stem().string();
    return filename;
}

std::vector<std::string> Path::get_common_folders(const Path& path) const {
    std::vector<std::string> tokens_relative_1;
    std::vector<std::string> tokens_relative_2;

    for (const auto& part : resource_path) {
        tokens_relative_1.push_back(part.string());
    }
    for (const auto& part : path.resource_path) {
        tokens_relative_2.push_back(part.string());
    }

    size_t minimum_size_tokens = std::min(tokens_relative_1.size(), tokens_relative_2.size());
    std::vector<std::string> common_folders;
    for (size_t i = 0; i < minimum_size_tokens; i++) {
        auto token_1 = tokens_relative_1[i];
        auto token_2 = tokens_relative_2[i];
        if (token_1 == token_2) {
            common_folders.push_back(token_1);
        } else {
            break;
        }
    }
    return common_folders;
}

bool Path::operator<(const Path& path) const {
    return resource_path < path.resource_path;
}

bool Path::contains_given_pattern(const std::string& pattern) const {
    std::string relative_path_plus_function_name = build_relative_path() / build_function_name();
    return relative_path_plus_function_name.find(pattern) != std::string::npos;
}

std::string Path::format_path_message_in_pair() const {
    return build_relative_path().string() + BETWEEN_RELATIVE_AND_FUNCTION_NAME + build_function_name();
}
