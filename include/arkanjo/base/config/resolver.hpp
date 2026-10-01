#pragma once

#include <cstdlib>
#include <optional>
#include <string_view>
#include <string>

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x) STRINGIFY_IMPL(x)

namespace arkanjo::config {
    /**
     * @brief Resolves a configuration value from environment variables with a fallback handler.
     *
     * @tparam F Callable type invoked with the optional environment variable value.
     * @param name Name of the environment variable.
     * @param fallback Callable receiving std::optional<std::string_view>.
     * @return Result of invoking the fallback callable.
     */
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
