#include "preprocessor_list.hpp"

#include <arkanjo/base/config/config.hpp>
#include <arkanjo/base/preprocess_state.hpp>

void PreprocessorList::print_containers(std::vector<ContainerInfo>& containers) {
    size_t color_offset = no_color ? 0 :10;
    std::cout << std::left
              << std::setw(29 + color_offset) << BOLD("CACHE CONTAINER") << "   "
              << std::setw(12 + color_offset) << BOLD("ID") << "   "
              << std::setw(10 + color_offset) << BOLD("DISK USAGE") << "   "
              << BOLD("EXTRA")
              << "\n";

    for (const auto& container : containers) {
        std::cout << std::left << std::setw(29 + color_offset + color_offset) << BOLD(BLUE(container.name)) << "   "
                  << std::left << std::setw(12) << container.id << "   "
                  << std::right << std::setw(10) << container.disk_usage << "   "
                  << std::left << container.extra
                  << "\n";
    }
}

PreprocessorList::PreprocessorList() { }

bool PreprocessorList::validate(const ParsedOptions& options) {
    no_color = options.args.count("no-color") > 0;
    return true;
}

bool PreprocessorList::run([[maybe_unused]] const ParsedOptions& options) {
    fs::path base_path = Config::config().base_path;

    std::vector<ContainerInfo> containers;

    for (const auto& entry : fs::directory_iterator(base_path)) {
        if (!fs::is_directory(entry)) continue;

        ContainerInfo container;
        container.name = entry.path().filename().string();

        Config::config().name_container = container.name;
        auto params = Preprocess_State::read_current_run_params();

        container.id = Utils::hash(container.name);

        container.disk_usage = Utils::format_size(params.size);
        if (params.size <= 0) {
            container.disk_usage = Utils::format_size(Utils::folder_size(entry.path()));
        }

        auto version = !params.version.empty() ? "v" + params.version : "";
        if (!Preprocess_State::is_cache_compatible(params.version)) {
            version = RED(version);
        }

        container.extra = version;
        containers.push_back(container);
    }

    print_containers(containers);

    return true;
}