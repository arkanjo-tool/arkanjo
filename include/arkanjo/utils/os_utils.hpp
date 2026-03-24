/**
 * @file os_utils.hpp
 * @brief utility functions
 *
 * Provides OS-dependent implementations for systems, including terminal
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
#include <vector>

class UtilsOSDependable {
    /**
     * @brief Gets the luminance of terminal background color
     *
     * @return float Luminance value between 0 (dark) and 1 (light)
     *
     * @note Uses terminal escape sequences to query background color
     */
    static float get_terminal_bg_color_luminance();

    /**
     * @brief Parses terminal color response into RGB components
     *
     * @param response Terminal color response string (format: "rgb:RRRR/GGGG/BBBB")
     * @return tuple<int, int, int> RGB components as 16-bit values (0-65535)
     *
     * @throws May throw runtime_error for malformed input
     */
    static std::tuple<int, int, int> parse_terminal_color_response(const std::string& response);

    /**
     * @brief Captures terminal response to color query
     *
     * @return string Raw terminal response containing color information
     *
     * @note Uses low-level terminal I/O to capture the response
     */
    static std::string capture_terminal_response();
public:
    /**
     * @brief Determines if terminal background color is dark
     *
     * @return bool True if background is dark (luminance < 0.5), false otherwise
     *
     * @note Uses get_terminal_bg_color_luminance() internally
     */
    static bool is_bg_color_dark();

    /**
     * @brief Executes an external program, replacing the current process (exec-style).
     *
     * This function launches an external command using the given executable name
     * and argument list. 
     *
     * @param cmd   Name of the executable (e.g., "git", "ls")
     * @param argv  Argument vector in C-style format (null-terminated).
     *              Example: {"git", "status", nullptr}
     *
     * @return int  Returns only on failure (e.g., -1). On success, does not return (POSIX).
     *
     * @note
     * - The command is searched in the system PATH.
     */
    static int run_process(const char* cmd, char* const argv[]);
}; // UtilsOSDependable
