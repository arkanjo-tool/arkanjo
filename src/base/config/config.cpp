#include <arkanjo/base/config/config.hpp>
#include <arkanjo/base/config/resolver.hpp>
#include <arkanjo/formatter/format_manager.hpp>

void Config::setTestConfig() {
    fs::path cwd = fs::current_path();

    base_path = cwd / "tests/e2e/current/tmp/arkanjo";
    third_party_dir = cwd / "third-party";
}

void Config::setDefaultConfig() {
    base_path = CONFIG_VALUE(ARKANJO_CACHE_PATH, [this](auto v) -> std::filesystem::path {
        if (v.has_value()) {
            return fs::path(*v) / program_name;
        }

        const char* cache_env = std::getenv("XDG_CACHE_HOME");
        const char* home = std::getenv("HOME");

        if (cache_env) {
            return fs::path(cache_env) / program_name;
        } else if (home) {
            return fs::path(home) / ".cache" / program_name;
        } else {
            FormatterManager::warn("neither XDG_CACHE_HOME nor HOME is set. Using default path.");
        }

        return base_path;
    });

    theme = CONFIG_VALUE(THEME_MODE, [](auto v) -> config::Theme {
        if (v.has_value()) {
            std::string_view mode = *v;
            return (mode == "dark") ? config::Theme::Dark : config::Theme::Light;
        }

        return UtilsOSDependable::is_bg_color_dark()
            ? config::Theme::Dark
            : config::Theme::Light;
    });

    third_party_dir = CONFIG_VALUE(ARKANJO_THIRD_PARTY_DIR, [this](auto v) -> std::filesystem::path {
        return v.has_value()
            ? fs::path(*v)
            : third_party_dir;
    });

    project_version = CONFIG_VALUE(ARKANJO_PROJECT_VERSION, [this](auto v) -> std::filesystem::path {
        return v.has_value()
            ? *v
            : project_version;
    });

    validate();
}

void Config::validate() {
    if (base_path.empty() || !fs::exists(third_party_dir) || !fs::is_directory(third_party_dir)) {
        throw std::runtime_error("Could not locate base-path directory");
    }

    if (!fs::exists(third_party_dir) || !fs::is_directory(third_party_dir)) {
        throw std::runtime_error("Could not locate third-party directory");
    }
}

Config& Config::config() {
    static Config instance;
    return instance;
}
