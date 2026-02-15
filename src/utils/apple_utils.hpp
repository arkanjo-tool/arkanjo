//

#pragma once

#include <sstream>
#include <string>

namespace UtilsOSDependable {

float get_terminal_bg_color_luminance();
std::tuple<int, int, int> parse_terminal_color_response(const std::string& response);
std::string capture_terminal_response();
bool is_bg_color_dark();
}; // namespace UtilsOSDependable
