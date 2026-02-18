#pragma once

#include <memory>
#include <string>
#include <arkanjo/utils/utils.hpp>

#define DEFINE_COLOR_HELPER(name)      \
inline std::string name(const std::string& text) { \
    auto fmt = FormatterManager::get_formatter();  \
    return fmt ? fmt->format(text, Utils::name) : text; \
}

class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual std::string format(const std::string& text, Utils::COLOR color) const = 0;
};

class ConsoleFormatter : public IFormatter {
public:
    ConsoleFormatter(bool enable_color = true) : color_enabled(enable_color) {}

    std::string format(const std::string& text, Utils::COLOR color) const override {
        if (!color_enabled) return text;
        return Utils::format_colored_message(text, color);
    }

private:
    bool color_enabled;
};

// Manager global
class FormatterManager {
public:
    static void set_formatter(std::shared_ptr<IFormatter> fmt) {
        formatter() = fmt;
    }

    static std::shared_ptr<IFormatter> get_formatter() {
        return formatter();
    }

private:
    static std::shared_ptr<IFormatter>& formatter() {
        static std::shared_ptr<IFormatter> f = std::make_shared<ConsoleFormatter>(true);
        return f;
    }
};

DEFINE_COLOR_HELPER(RED)
DEFINE_COLOR_HELPER(GREEN)
DEFINE_COLOR_HELPER(YELLOW)
DEFINE_COLOR_HELPER(BLUE)
DEFINE_COLOR_HELPER(CYAN)
DEFINE_COLOR_HELPER(MAGENTA)
DEFINE_COLOR_HELPER(GRAY)
DEFINE_COLOR_HELPER(WHITE)