#include <arkanjo/commands/pre/preprocessor.hpp>

#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/config.hpp>

void Preprocessor::save_current_run_params(const fs::path& path) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string time_str(std::ctime(&end_time));
    std::string version = PROJECT_VERSION;
    if (!time_str.empty() && time_str.back() == '\n')
        time_str.pop_back();

    json data = PreprocessRunParams{path, time_str, version};

    fs::path config_path = Config::config().base_path / Config::config().name_container / CONFIG_PATH;
    Utils::write_file(config_path, data.dump(4) + '\n');
}

PreprocessRunParams Preprocessor::read_current_run_params() {
    fs::path file_path = Config::config().base_path / Config::config().name_container / CONFIG_PATH;

    std::ifstream infile(file_path);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open " << file_path << "\n";
        return {};
    }

    json data = json::parse(infile, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Error: could not parse " << file_path << "\n";
        return {};
    }

    return data.get<PreprocessRunParams>();
}

constexpr int MAX_MINOR_VERSION_DIFF = 1;

bool Preprocessor::is_cache_compatible(const std::string& cache_version) {
    int current_major, current_minor, current_patch;
    int cache_major, cache_minor, cache_patch;
    char dot;

    std::istringstream(PROJECT_VERSION)
        >> current_major >> dot >> current_minor >> dot >> current_patch;

    std::istringstream(cache_version)
        >> cache_major >> dot >> cache_minor >> dot >> cache_patch;

    return (current_major == cache_major && std::abs(current_minor - cache_minor) <= MAX_MINOR_VERSION_DIFF);
}