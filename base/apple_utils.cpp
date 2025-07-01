#include "apple_utils.hpp"

#ifdef __APPLE__
#include <TargetConditionals.h>

tuple<int, int, int> UtilsOSDependable::parse_terminal_color_response(){

}

float UtilsOSDependable::get_terminal_bg_color_luminance(){
  auto [r, g, b] = parse_terminal_color_response();
  return 0.2126 * (r/255) + 0.7152 * (g/255) + 0.0722 * (b/255);
}

bool UtilsOSDependable::is_bg_color_dark(){
  return get_terminal_bg_color_luminance() <= 0.5;
}

#endif