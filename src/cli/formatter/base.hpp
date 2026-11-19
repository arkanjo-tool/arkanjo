#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include "cli/formatter/theme.hpp"

/**
 * @brief Output presentation formats.
 */
enum class Format {
    TEXT, ///< Human-readable styled text format.
    JSON, ///< Structured JSON format.
    AUTO  ///< Automatic selection based on context/terminal.
};

using StyleMap = std::unordered_map<std::string, formatter::Color>;
using RowColorFn = std::function<formatter::Color(size_t)>;

/**
 * @brief Interface for formatting and coloring text output.
 */
class IFormatter {
public:
    virtual ~IFormatter() = default;

    /**
     * @brief Formats text with the specified color.
     *
     * @param text Text string to format.
     * @param color Color code to apply.
     *
     * @return Formatted string with ANSI escape codes or plaintext.
     */
    virtual std::string colorize(const std::string& text, formatter::Color color) const = 0;

    /**
     * @brief Returns the active theme style map.
     *
     * @return Reference to StyleMap.
     */
    virtual const StyleMap& style() const = 0;
};

/**
 * @brief Terminal console formatter supporting ANSI colors and customizable themes.
 */
class ConsoleFormatter : public IFormatter {
public:
    explicit ConsoleFormatter(
        bool enable_color = true
    )
        : enabled(enable_color) {}

    std::string colorize(const std::string& text, formatter::Color color) const override {
        if (!enabled) return text;
        return formatter::format_colored_message(text, color);
    }

    const StyleMap& style() const override {
        return theme;
    }

    void set(const std::string& key, formatter::Color c) {
        theme[key] = c;
    }

    void set_theme(const StyleMap& t) {
        theme = t;
    }

private:
    bool enabled;
    StyleMap theme = {
        {"file", formatter::Color::RED},
        {"function", formatter::Color::BOLD},
        {"number", formatter::Color::YELLOW},
        {"bold", formatter::Color::BOLD},
        {"warning", formatter::Color::MAGENTA},
        {"error", formatter::Color::RED},
        {"row_even", formatter::Color::GRAY},
        {"row_odd", formatter::Color::CYAN},
    };
};
