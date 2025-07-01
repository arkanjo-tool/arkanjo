/**
 * @file windows_utils.hpp
 * @brief Windows-specific utility functions
 * 
 * Provides OS-dependent implementations for Windows systems, including terminal
 * color detection utilities.
 */

#ifndef WINDOWS_UTILS_HPP
#define WINDOWS_UTILS_HPP

#pragma once
#include <string>
#include <sstream>

using namespace std;

namespace UtilsOSDependable{
        /**
         * @brief Gets the luminance of terminal background color on Windows
         * 
         * @return float Luminance value between 0 (dark) and 1 (light)
         * 
         * @note Uses Windows Console API to determine background color
         * @note Defaults to dark if unable to determine color
         */
        float get_terminal_bg_color_luminance();
        
        /**
         * @brief Parses terminal color response into RGB components on Windows
         * 
         * @return tuple<int, int, int> RGB components as 8-bit values (0-255)
         * 
         * @note Uses Windows Console API to get color information
         * @note Returns default dark theme colors if unable to determine
         */
        tuple<int, int, int> parse_terminal_color_response();
        
        /**
         * @brief Determines if terminal background color is dark on Windows
         * 
         * @return bool True if background is dark (luminance < 0.5), false otherwise
         * 
         * @note Uses get_terminal_bg_color_luminance() internally
         * @note Defaults to true (dark) if unable to determine color
         */
        bool is_bg_color_dark();
};

#endif