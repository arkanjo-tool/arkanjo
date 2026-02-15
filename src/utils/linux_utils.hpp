/**
 * @file linux_utils.hpp
 * @brief Linux-specific utility functions
 *
 * Provides OS-dependent implementations for Linux systems, including terminal
 * color detection and conversion utilities.
 */

#pragma once
#include <algorithm>
#include <cstdlib>
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

namespace UtilsOSDependable {
/**
 * @brief Converts a 16-bit hex color string to 8-bit RGB components
 *
 * @param hex16 16-bit hex color string (format: "RRGGBB" or "#RRGGBB")
 * @return int 8-bit color value (0-255)
 *
 * @note Input string can be with or without leading '#'
 */
int convert_16_bit_to_8_bit(const std::string& hex16);

/**
 * @brief Gets the luminance of terminal background color
 *
 * @return float Luminance value between 0 (dark) and 1 (light)
 *
 * @note Uses terminal escape sequences to query background color
 */
float get_terminal_bg_color_luminance();

/**
 * @brief Parses terminal color response into RGB components
 *
 * @param response Terminal color response string (format: "rgb:RRRR/GGGG/BBBB")
 * @return tuple<int, int, int> RGB components as 16-bit values (0-65535)
 *
 * @throws May throw runtime_error for malformed input
 */
std::tuple<int, int, int> parse_terminal_color_response(const std::string& response);

/**
 * @brief Captures terminal response to color query
 *
 * @return string Raw terminal response containing color information
 *
 * @note Uses low-level terminal I/O to capture the response
 */
std::string capture_terminal_response();

/**
 * @brief Determines if terminal background color is dark
 *
 * @return bool True if background is dark (luminance < 0.5), false otherwise
 *
 * @note Uses get_terminal_bg_color_luminance() internally
 */
bool is_bg_color_dark();
}; // namespace UtilsOSDependable
