#include <arkanjo/base/function/function_printer.hpp>
#include <arkanjo/base/preprocess_state.hpp>

#include <arkanjo/formatter/format_manager.hpp>

void FunctionPrinter::print_basic_info(const Function& function) {
    auto params = Preprocess_State::read_current_run_params();
    auto location = function.scope_location();

    std::string function_message = FUNCTION_PREFIX_PRINT + function.path().build_function_name();
    fs::path relative_path = params.path / fs::path{function.path().build_relative_path()};
    std::string relative_message = RELATIVE_PATH_PRINT + relative_path.string();
    std::string start_message = LINE_DECLARATION_PRINT + std::to_string(location.declaration + 1) +
        "-" + std::to_string(location.end + 1);
    std::string number_message = NUMBER_LINE_PRINT + std::to_string(location.size());

    std::cout << YELLOW(function_message) << '\n';
    std::cout << GREEN(relative_message) << '\n';
    std::cout << WHITE(start_message) << '\n';
    std::cout << WHITE(number_message) << '\n';
}

void FunctionPrinter::print_code(const Function& function, bool no_numbers) {
    auto lines = function.build_all_content();

    int line_number = function.scope_location().declaration + 1;

    for (const auto& line : lines) {
        if (!no_numbers) {
            std::ostringstream oss;
            oss << std::setw(4) << line_number++;

            std::cout << CYAN(oss.str()) << " | " << line << '\n';
        } else {
            std::cout << line << '\n';
        }
    }
}
