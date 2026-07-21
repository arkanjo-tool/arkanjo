#include <arkanjo/base/path.hpp>

Path::Path(const fs::path& _path) {
    resource_path = _path;
}

const fs::path& Path::value() const {
    return resource_path;
}

bool Path::empty() const {
    return resource_path.empty();
}

fs::path Path::relative_path() const {
    return resource_path.parent_path();
}

std::vector<std::string> Path::common_folders(const Path& path) const {
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
