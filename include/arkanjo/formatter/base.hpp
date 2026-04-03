#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <arkanjo/utils/utils.hpp>

enum class Format {
    TEXT,
    JSON,
    AUTO
};

using StyleMap = std::unordered_map<std::string, Utils::COLOR>;
using RowColorFn = std::function<Utils::COLOR(size_t)>;

class IFormatter {
public:
    virtual ~IFormatter() = default;
    virtual std::string colorize(
        const std::string& text, 
        Utils::COLOR color
    ) const = 0;
    virtual const StyleMap& style() const = 0;
};

class ConsoleFormatter : public IFormatter {
public:
    explicit ConsoleFormatter(
        bool enable_color = true
    )
        : enabled(enable_color) {}

    std::string colorize(
        const std::string& text,
        Utils::COLOR color
    ) const override {
        if (!enabled) return text;
        return Utils::format_colored_message(text, color);
    }

    const StyleMap& style() const override {
        return theme;
    }

    void set(const std::string& key, Utils::COLOR c) {
        theme[key] = c;
    }

    void set_theme(const StyleMap& t) {
        theme = t;
    }

private:
    bool enabled;
    StyleMap theme = {
        {"file", Utils::COLOR::RED},
        {"function", Utils::COLOR::BOLD},
        {"number", Utils::COLOR::YELLOW},
        {"bold", Utils::COLOR::BOLD},
        {"warning", Utils::COLOR::MAGENTA},
        {"row_even", Utils::COLOR::GRAY},
        {"row_odd", Utils::COLOR::CYAN},
    };
};
