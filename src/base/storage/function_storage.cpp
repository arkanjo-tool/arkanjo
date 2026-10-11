#include <arkanjo/base/config/config.hpp>
#include <arkanjo/base/storage/function_storage.hpp>

fs::path FunctionStorage::build_source_path(const Path& path) const {
    auto& cfg = Config::config();
    return cfg.base_path / cfg.name_container / cfg.source_path / path.value();
}

fs::path FunctionStorage::build_header_path(const Path& path) const {
    auto& cfg = Config::config();
    return cfg.base_path / cfg.name_container / cfg.header_path / path.value();
}

fs::path FunctionStorage::build_info_path(const Path& path) const {
    auto& cfg = Config::config();
    fs::path p = cfg.base_path / cfg.name_container / cfg.info_path / path.value();
    p.replace_extension(JSON_EXTENSION);
    return p;
}

fs::path FunctionStorage::build_similarity_path() const {
    auto& cfg = Config::config();
    return cfg.base_path / cfg.name_container / cfg.similarity_path;
}

fs::path FunctionStorage::build_output_path(
    const fs::path type_path, const fs::path& relative_path, const std::string& function_name
) const {
    auto& cfg = Config::config();
    fs::path base = cfg.base_path / cfg.name_container / type_path;

    std::string ext;
    if (type_path == cfg.info_path) {
        ext = "json";
    } else {
        ext = relative_path.extension().string();
        if (!ext.empty() && ext[0] == '.')
            ext.erase(0, 1);
    }

    return base / relative_path / (function_name + "." + ext);
}
