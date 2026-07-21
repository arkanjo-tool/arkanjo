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
