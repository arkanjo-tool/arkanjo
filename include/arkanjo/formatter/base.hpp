#pragma once

#include <unordered_map>
#include <functional>
#include <string>

#include <arkanjo/utils/utils.hpp>

/**
 * @brief Output presentation formats.
 */
enum class Format {
    TEXT, ///< Human-readable styled text format.
    JSON, ///< Structured JSON format.
    AUTO  ///< Automatic selection based on context/terminal.
};

using StyleMap = std::unordered_map<std::string, Utils::COLOR>;
using RowColorFn = std::function<Utils::COLOR(size_t)>;

/**
 * @brief Interface for formatting and coloring text output.
 */
class IFormatter {
public:
    virtual ~IFormatter() = default;

    /**
     * @brief Formats text with the specified color.
     * @param text Text string to format.
     * @param color Color code to apply.
     * @return Formatted string with ANSI escape codes or plaintext.
     */
    virtual std::string colorize(
        const std::string& text, 
        Utils::COLOR color
    ) const = 0;

    /**
     * @brief Returns the active theme style map.
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
        {"primary",   Utils::COLOR::BLUE},
        {"secondary", Utils::COLOR::CYAN},
        {"success",   Utils::COLOR::GREEN},
        {"warning",   Utils::COLOR::YELLOW},
        {"error",     Utils::COLOR::RED},
        {"muted",     Utils::COLOR::GREY},
    };
};
