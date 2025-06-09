#include "windows_utils.hpp"

#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

tuple<int, int, int> UtilsOSDependable::parse_terminal_color_response(){
  HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
  HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

  // Save original console mode
  DWORD originalOutMode = 0;
  GetConsoleMode(hOutput, &originalOutMode);

  // Enable virtual terminal processing
  SetConsoleMode(hOutput, originalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

  // Set input to raw mode (disable line input and echo input)
  DWORD originalInMode = 0;
  GetConsoleMode(hInput, &originalInMode);
  SetConsoleMode(hInput, originalInMode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));

  // Send OSC 11 query for background color
  const char* query = "\x1b]11;?\x1b\\";
  DWORD written = 0;
  WriteConsoleA(hOutput, query, (DWORD)strlen(query), &written, nullptr);

  // Read raw input
  std::string response;
  char buffer[256];
  DWORD read = 0;
  for (int i = 0; i < 20; ++i) {
      if (ReadFile(hInput, buffer, sizeof(buffer), &read, nullptr) && read > 0) {
          response.append(buffer, read);
          if (response.find('\x1b') != std::string::npos && response.find("rgb:") != std::string::npos) {
              break;
          }
      }
      Sleep(50);
  }

  // Restore original modes
  SetConsoleMode(hInput, originalInMode);
  SetConsoleMode(hOutput, originalOutMode);

  // Parse RGB from response
  size_t start = response.find("rgb:");
  if (start == std::string::npos) return false;
  start += 4;

  size_t end = response.find("/", start);
  if (end == std::string::npos) return false;
  std::string rStr = response.substr(start, end - start);

  start = end + 1;
  end = response.find("/", start);
  if (end == std::string::npos) return false;
  std::string gStr = response.substr(start, end - start);

  start = end + 1;
  end = response.find("\x1b", start);
  if (end == std::string::npos) return false;
  std::string bStr = response.substr(start, end - start);

  try {
      r = std::stoi(rStr, nullptr, 16) / 256;
      g = std::stoi(gStr, nullptr, 16) / 256;
      b = std::stoi(bStr, nullptr, 16) / 256;
      return {r, g, b};
  } catch (...) {
      return {0, 0, 0};
  }
}

float UtilsOSDependable::get_terminal_bg_color_luminance(){
  auto [r, g, b] = parse_terminal_color_response();
  float luminance = 0.2126 * (r/255.0) + 0.7152 * (g/255.0) + 0.0722 * (b/255.0);

  return luminance;
}

bool UtilsOSDependable::is_bg_color_dark(){
  return get_terminal_bg_color_luminance() <= 0.5;
}

#endif
