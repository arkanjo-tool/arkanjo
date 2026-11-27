#pragma once

#include <string>
#include <string_view>

namespace formatter {

/**
 * @brief ANSI color codes for light terminal backgrounds
 *
 * Contains escape sequences for:
 * - RESET
 * - RED
 * - GREEN
 * - YELLOW
 * - BLUE
 * - MAGENTA
 * - CYAN
 * - GRAY
 * - WHITE
 * - BRIGHT_YELLOW
 * - BLACK
 * - BOLD
 * - UNDERLINE
 */
static constexpr std::string_view kColorTokensUtilsLight[] = {
    "\033[0m",    // RESET
    "\033[31;2m", // DARK_RED
    "\033[32;2m", // DARK_GREEN
    "\033[33;2m", // DARK_YELLOW
    "\033[34;2m", // DARK_BLUE
    "\033[35;2m", // DARK_MAGENTA
    "\033[36;2m", // DARK_CYAN
    "\033[37;2m", // DARK_GRAY
    "\033[30;2m", // BLACK
    "\033[33;2m", // YELLOW
    "\033[30m",   // BLACK
    "\033[01m",   // BOLD
    "\033[04m",   // UNDERLINE
    "",           // NONE
};

/**
 * @brief ANSI color codes for dark terminal backgrounds
 *
 * Contains escape sequences for:
 * - RESET
 * - DARK_RED
 * - DARK_GREEN
 * - DARK_YELLOW
 * - DARK_BLUE
 * - DARK_MAGENTA
 * - DARK_CYAN
 * - DARK_GRAY
 * - BLACK
 * - YELLOW
 * - WHITE
 */
static constexpr std::string_view kColorTokensUtilsDark[] = {
    "\033[00m", // RESET
    "\033[31m", // RED
    "\033[32m", // GREEN
    "\033[33m", // YELLOW
    "\033[34m", // BLUE
    "\033[35m", // MAGENTA
    "\033[36m", // CYAN
    "\033[37m", // GRAY
    "\033[97m", // WHITE
    "\033[33m", // BRIGHT_YELLOW
    "\033[97m", // WHITE
    "\033[01m", // BOLD
    "\033[04m", // UNDERLINE
    "",         // NONE
};

/**
 * @brief Enumeration of available colors for formatted messages
 */
enum class Color {
    RESET,         ///< Reset to default color
    RED,           ///< Red color
    GREEN,         ///< Green color
    YELLOW,        ///< Yellow color
    BLUE,          ///< Blue color
    MAGENTA,       ///< Magenta color
    CYAN,          ///< Cyan color
    GRAY,          ///< Gray color
    WHITE,         ///< White color
    BRIGHT_YELLOW, ///< Bright yellow color
    BLACK,         ///< Black color
    BOLD,          ///< Bold text
    UNDERLINE,     ///< Underline text
    NONE
};

/**
 * @brief Formats a message with ANSI color codes
 * @param message Text to colorize
 * @param color Color to apply
 * @return string Colorized message with reset code at the end
 */
std::string format_colored_message(const std::string& message, Color color);

} // namespace formatter
