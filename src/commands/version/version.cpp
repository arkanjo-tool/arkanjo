#include "version.hpp"

#include <arkanjo/base/config/config.hpp>

using fm = FormatterManager;

Version::Version([[maybe_unused]] Similarity_Table* _similarity_table) { }

bool Version::validate([[maybe_unused]] const ParsedOptions& options) {
    return true;
}

bool Version::run([[maybe_unused]] const ParsedOptions& options) {
    std::string version = Config::config().project_version;
    fm::write(version);

    return true;
}