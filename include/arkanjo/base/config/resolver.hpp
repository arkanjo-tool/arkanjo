#pragma once

#include <cstdlib>
#include <optional>
#include <string_view>
#include <string>

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

namespace arkanjo::config {
    template<typename F>
    auto resolve_config_impl(std::string_view name, F&& fallback) {
        std::string name_cstr{name};
        if (const char* env = std::getenv(name_cstr.c_str())) {
            return fallback(std::optional<std::string_view>(env)); 
        }
        return fallback(std::optional<std::string_view>{std::nullopt});
    }
}

#define CONFIG_VALUE(name, fallback_lambda)                                         \
    ([](auto&& f) {                                                                   \
        if constexpr (std::string_view(#name) != std::string_view(STRINGIFY(name))) { \
            return f(std::optional<std::string_view>{STRINGIFY(name)});               \
        } else {                                                                      \
            return arkanjo::config::resolve_config_impl(#name, f);                    \
        }                                                                             \
    })(fallback_lambda)
