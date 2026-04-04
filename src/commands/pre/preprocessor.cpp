#include <arkanjo/commands/pre/preprocessor.hpp>

#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/config.hpp>

#include <vector>
#include <string>

void Preprocessor::save_current_run_params(const fs::path& path) {
    std::vector<std::string> config_content;

    std::string path_message = PATH_MESSAGE + path.string();

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string time_message = TIME_MESSAGE + std::string(std::ctime(&end_time));

    config_content.push_back(path_message);
    config_content.push_back(time_message);

    Utils::write_file_generic(Config::config().base_path / Config::config().name_container / CONFIG_PATH, config_content);
}