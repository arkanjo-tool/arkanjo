#pragma once

#include <filesystem>
#include <string>

namespace config {
  enum class Theme {
    Light,
    Dark
  };
}

namespace arkanjo {
  struct ConfigSchema {
    std::string program_name = "arkanjo";                  ///< Name of the program
    std::filesystem::path base_path = "tmp/arkanjo";       ///< Default base path for temporary files
    std::filesystem::path third_party_dir = "third-party"; ///< Directory containing third-party dependencies
    std::filesystem::path name_container = "default";      ///< Name of the cache container
    std::string project_version;                           ///< Current project version

    std::filesystem::path source_path = "source";          ///< Source subdirectory name
    std::filesystem::path header_path = "header";          ///< Header subdirectory name
    std::filesystem::path info_path = "info";              ///< Info subdirectory name

    config::Theme theme = config::Theme::Dark;             ///< Current terminal theme
  };
}