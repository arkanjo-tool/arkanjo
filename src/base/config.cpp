#include <arkanjo/base/config.hpp>
#include <arkanjo/utils/utils.hpp>
#include <iostream>

void Config::setTestConfig() {
    base_path = "tests/e2e/current/tmp/arkanjo";
}

void Config::setDefaultConfig() {
    // BASE PATH
    const char* cache_env = std::getenv("XDG_CACHE_HOME");
    const char* home = std::getenv("HOME");

    if (cache_env) {
        base_path = fs::path(cache_env) / program_name;
    } else if (home) {
        base_path = fs::path(home) / ".cache" / program_name;
    } else {
        std::cerr << "Warning: neither XDG_CACHE_HOME nor HOME is set. Using default path.\n";
    }

    // THEME
    theme = UtilsOSDependable::is_bg_color_dark()
        ? config::Theme::Dark
        : config::Theme::Light;
}

Config& Config::config() {
    static Config instance;
    return instance;
}
