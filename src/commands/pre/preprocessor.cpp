#include <arkanjo/commands/pre/preprocessor.hpp>

#include <arkanjo/utils/utils.hpp>
#include <arkanjo/base/config.hpp>

void Preprocessor::save_current_run_params(const fs::path& path) {
    std::vector<std::string> config_content;

    std::string path_message = PATH_MESSAGE + path.string();

    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    std::string time_message = TIME_MESSAGE + std::string(std::ctime(&end_time));

    config_content.push_back(path_message);
    config_content.push_back(time_message);

    Utils::write_file_with_vector(Config::config().base_path / Config::config().name_container / CONFIG_PATH, config_content);
}

std::vector<std::string> Preprocessor::read_current_run_params() {
    std::vector<std::string> content;
    fs::path file_path = Config::config().base_path / Config::config().name_container / CONFIG_PATH;

    std::ifstream infile(file_path);
    if (!infile.is_open()) {
        std::cerr << "Error: could not open " << file_path << "\n";
        return content;
    }

    std::string line;
    while (std::getline(infile, line)) {
        if (line.rfind(PATH_MESSAGE, 0) == 0) {
            line = line.substr(std::string(PATH_MESSAGE).size());
        }
        else if (line.rfind(TIME_MESSAGE, 0) == 0) {
            line = line.substr(std::string(TIME_MESSAGE).size());
        }

        content.push_back(line);
    }

    return content;
}