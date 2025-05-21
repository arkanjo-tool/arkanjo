#include "linux_utils.hpp"

#include <sys/utsname.h>

int UtilsOSDependable::convert_16_bit_to_8_bit(const std::string& hex16){
  try{
      unsigned long value = std::stoul(hex16, nullptr, 16);
      return static_cast<int>(value / 256);  
  } catch (...){
      return 0; 
  }
}

tuple<int, int, int> UtilsOSDependable::parse_terminal_color_response(const string& response){
  const size_t start_pos = response.find("rgb:");
  if (start_pos == string::npos){
      return {0, 0, 0};
  }

  const size_t end_pos = response.find("\033\\", start_pos);
  if (end_pos == string::npos){
      return {0, 0, 0};
  }

  string rgb_str = response.substr(start_pos + 4, end_pos - (start_pos + 4));

  replace(rgb_str.begin(), rgb_str.end(), '/', ' ');

  istringstream iss(rgb_str);
  string r_hex, g_hex, b_hex;
  iss >> r_hex >> g_hex >> b_hex;

  int r = convert_16_bit_to_8_bit(r_hex);
  int g = convert_16_bit_to_8_bit(g_hex);
  int b = convert_16_bit_to_8_bit(b_hex);

  return {r, g, b};
}

string UtilsOSDependable::capture_terminal_response(){
  termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  cout << "\033]11;?\033\\";
  cout.flush();

  string response;
  char ch;
  timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 100000; // 100ms timeout

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(STDIN_FILENO, &readfds);

  while (select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv) > 0){
      if (read(STDIN_FILENO, &ch, 1) > 0){
          response += ch;
      } else{
          break;
      }
  }

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  return response;
}

float UtilsOSDependable::get_terminal_bg_color_luminance(){
  if (!isatty(STDOUT_FILENO)){
      return 0;
  }

  string color_str = capture_terminal_response();
  auto [r, g, b] = parse_terminal_color_response(color_str);
  
  return 0.2126 * (r/255) + 0.7152 * (g/255) + 0.0722 * (b/255);
}

bool UtilsOSDependable::is_bg_color_dark(){
    return get_terminal_bg_color_luminance() <= 0.5;
}
