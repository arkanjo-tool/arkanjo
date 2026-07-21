#include <arkanjo/base/function/function_loader.hpp>
#include <arkanjo/base/path.hpp>
#include <arkanjo/base/storage/function_storage.hpp>

#include <arkanjo/utils/utils.hpp>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

void FunctionLoader::read_content(Function& function) {
    FunctionStorage storage;

    fs::path source_path = storage.build_source_path(function.path());
    function.content_ = Utils::read_file_with_vector(source_path);
}

void FunctionLoader::read_header(Function& function) {
    FunctionStorage storage;

    fs::path header_path = storage.build_header_path(function.path());
    function.header_ = Utils::read_file_with_vector(header_path);
}

void FunctionLoader::read_info(Function& function) {
    FunctionStorage storage;

    fs::path info_path = storage.build_info_path(function.path());
    json info = Utils::read_json(info_path);
    function.location_.declaration = info.value(LINE_DECLARATION_JSON, INVALID_LINE);
    function.location_.begin = info.value(START_NUMBER_LINE_JSON, INVALID_LINE);
    function.location_.end = info.value(END_NUMBER_LINE_JSON, INVALID_LINE);
}

Function FunctionLoader::load(const Path& path) {
    if (path.empty())
        return {};

    Function function(path);

    function.name_ = path.value().stem().string();

    read_content(function);
    read_header(function);
    read_info(function);

    return function;
}

Function FunctionLoader::load_metadata(const Path& path) {
    if (path.empty())
        return {};

    Function function(path);

    function.name_ = path.value().stem().string();

    read_info(function);

    return function;
}
