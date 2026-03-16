#include <arkanjo/base/config.hpp>

void Config::setTestConfig() {
    base_path = "tests/e2e/current/tmp";
}

void Config::setDefaultConfig() {
    const char* cache_env = std::getenv("XDG_CACHE_HOME");

    if (cache_env) {
        base_path = fs::path(cache_env) / program_name;
    } else {
        const char* home = std::getenv("HOME");
        base_path = fs::path(home) / ".cache" / program_name;
    }
}

Config& Config::config() {
    static Config instance;
    return instance;
}
