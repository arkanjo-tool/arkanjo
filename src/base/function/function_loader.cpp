#include <arkanjo/base/function/function_loader.hpp>
#include <arkanjo/base/path.hpp>

#include <arkanjo/utils/utils.hpp>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

void FunctionLoader::read_content(Function& function) {
    fs::path source_path = function.path().build_source_path();
    function.content_ = Utils::read_file_with_vector(source_path);
}

void FunctionLoader::read_header(Function& function) {
    fs::path header_path = function.path().build_header_path();
    function.header_ = Utils::read_file_with_vector(header_path);
}

void FunctionLoader::read_info(Function& function) {
    fs::path info_path = function.path().build_info_path();
    json info = Utils::read_json(info_path);
    function.location_.declaration = info.value(LINE_DECLARATION_JSON, INVALID_LINE);
    function.location_.begin = info.value(START_NUMBER_LINE_JSON, INVALID_LINE);
    function.location_.end = info.value(END_NUMBER_LINE_JSON, INVALID_LINE);
}

Function FunctionLoader::load(const Path& path) {
    if (path.is_empty())
        return {};

    Function function(path);

    read_content(function);
    read_header(function);
    read_info(function);

    return function;
}
