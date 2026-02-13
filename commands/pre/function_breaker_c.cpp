// TODO - line declaration extraction does not work really well to extract definition
// because of comments or define in the middle of definition

#include "function_breaker_c.hpp"

bool FunctionBreakerC::is_define(int line, int pos) {
    int line_size = file_content.size();
    // does not fit the #define token
    if (pos + 7 > line_size)
        return false;
    // match the token
    string token = "#define";
    bool match = true;
    for (int j = 0; j < 7; j++) {
        match &= file_content[line][pos + j] == token[j];
    }
    return match;
}

// Only works if the code is compilable. I do have grant any
// ensurances if the source code does not compile
void FunctionBreakerC::filter_mask_commentaries_and_defines(vector<vector<bool>>& mask) {
    // aqui tbm tem que lidar com string literal, ie, "#define" nao eh define a
    // eh "//" nao eh commentario
    int number_lines = file_content.size();
    bool is_open_block_comment = false;
    bool is_open_define = false;
    bool is_open_quotation_marks = false;
    bool is_open_line_comment = false;

    for (int i = 0; i < number_lines; i++) {
        auto& line = file_content[i];
        auto& mask_line = mask[i];
        int line_size = line.size();

        if (is_open_define) {
            for (int j = 0; j < line_size; j++) {
                mask_line[j] = false;
            }
            // if the last token is to continue the define
            if (line.back() != '\\') {
                is_open_define = false;
            }
            continue;
        }

        if (is_open_line_comment) {
            for (int j = 0; j < line_size; j++) {
                mask_line[j] = false;
            }
            // if the last token is to continue the define
            if (line.back() != '\\') {
                is_open_line_comment = false;
            }
            continue;
        }

        for (int j = 0; j < line_size; j++) {
            if (is_open_block_comment) {
                mask_line[j] = false;
                // if the block line comes to an end
                if (j + 1 < line_size && line[j] == '*' && line[j + 1] == '/') {
                    j++;
                    mask_line[j] = false;
                    is_open_block_comment = false;
                }
                continue;
            }

            if (is_open_quotation_marks) {
                mask_line[j] = false;
                // TODO should I take a look on ""s ?
                if (line[j] == '"') {
                    is_open_quotation_marks = false;
                } else if (line[j] == '\\') {
                    if (j == line_size - 1) {
                        break;
                    } else {
                        j++;
                        mask_line[j] = false;
                    }
                }
                continue;
            }

            if (line[j] == '\'') {
                assert(j + 1 < line_size &&
                       "source code does not compile, ' open but not closed");
                mask_line[j] = false;
                j++;
                if (line[j] == '\\') {
                    assert(j + 2 < line_size && line[j + 2] == '\'' &&
                           "source code does not compile, ' open but not closed");
                    mask_line[j] = false;
                    j++;
                } else {
                    assert(j + 1 < line_size && line[j + 1] == '\'' &&
                           "source code does not compile, ' open but not closed");
                }
                mask_line[j] = false;
                j++;
                mask_line[j] = false;
                continue;
            }

            if (line[j] == '"') {
                is_open_quotation_marks = true;
                mask_line[j] = false;
                continue;
            }

            if (line[j] == '/') {
                if (j == line_size - 1) {
                    continue;
                }

                if (line[j + 1] == '/') {
                    for (int k = j; k < line_size; k++) {
                        mask_line[k] = false;
                    }
                    // find line comment, everything after is comment and
                    // break the iteration on the current line
                    is_open_line_comment = line.back() == '\\';
                    break;
                }

                if (line[j + 1] == '*') {
                    mask_line[j] = false;
                    j++;
                    mask_line[j] = false;
                    is_open_block_comment = true;
                    continue;
                }
            }

            if (is_define(i, j)) {
                for (int k = j; k < line_size; k++) {
                    mask_line[k] = false;
                }
                // find #define, everything after is comment and
                // break the iteration on the current line
                is_open_define = line.back() == '\\';
                break;
            }
        }
    }

    assert(is_open_block_comment == false &&
           "source code does not compile, open block comment");
    assert(is_open_quotation_marks == false &&
           "source code does not compile, open quotation marks");
}

// the exactly same size of the input source, the character will be 1 if it is not in a commentary nor a #define's
vector<vector<bool>> FunctionBreakerC::build_mask_valid_code() {
    vector<vector<bool>> mask(file_content.size());
    for (int i = 0; i < (int)file_content.size(); i++) {
        mask[i] = vector<bool>(file_content[i].size(), true);
    }
    filter_mask_commentaries_and_defines(mask);
    return mask;
}

set<array<int, 5>> FunctionBreakerC::find_start_end_and_depth_of_brackets() {
    set<array<int, 5>> start_ends;
    int open_brackets = 0;

    vector<pair<int, int>> not_processed_open_brackets;
    auto process_open = [&](int line_number, int column) {
        open_brackets++;
        not_processed_open_brackets.push_back({line_number, column});
    };
    auto process_close = [&](int line_number, int column) {
        open_brackets--;
        if (open_brackets <= -1) {
            open_brackets = 0;
        } else {
            auto [matched_line, matched_column] = not_processed_open_brackets.back();
            not_processed_open_brackets.pop_back();
            int depth_of_open = not_processed_open_brackets.size();
            start_ends.insert({matched_line,
                matched_column,
                line_number,
                column,
                depth_of_open});
        }
    };

    for (size_t i = 0; i < file_content.size(); i++) {
        auto& line = file_content[i];
        for (size_t j = 0; j < line.size(); j++) {
            if (!mask_valid[i][j]) {
                continue;
            }
            auto c = line[j];
            if (c == '{') {
                process_open(i, j);
            }
            if (c == '}') {
                process_close(i, j);
            }
        }
    }
    return start_ends;
}

set<array<int, 4>> FunctionBreakerC::find_start_end_of_brackets_of_given_depth() {
    set<array<int, 4>> ret;
    set<array<int, 5>> bracket_pairs = find_start_end_and_depth_of_brackets();
    for (auto [start_line, start_column, end_line, end_column, dep] : bracket_pairs) {
        if (dep == C_RELEVANT_DEPTH) {
            ret.insert({start_line, start_column, end_line, end_column});
        }
    }
    return ret;
}

vector<string> FunctionBreakerC::build_function_content(int start_number_line, int start_column, int end_number_line, int end_column) {
    vector<string> function_content;

    if (start_number_line == end_number_line) {
        string line = "";
        for (int j = start_column; j <= end_column; j++) {
            line += file_content[start_number_line][j];
        }
        function_content.push_back(line);
        return function_content;
    }

    string first_line = file_content[start_number_line];
    int first_line_size = first_line.size();
    string first_line_contribution = "";
    for (int j = start_column; j < first_line_size; j++) {
        first_line_contribution += first_line[j];
    }
    function_content.push_back(first_line_contribution);

    for (int i = start_number_line + 1; i < end_number_line; i++) {
        function_content.push_back(file_content[i]);
    }

    string last_line = file_content[end_number_line];
    int last_line_size = last_line.size();
    string last_line_contribution = "";
    for (int j = 0; j <= end_column; j++) {
        last_line_contribution += last_line[j];
    }
    function_content.push_back(last_line_contribution);

    return function_content;
}

bool FunctionBreakerC::move_pointer_until_character_outside_parenteses(int& line, int& column) {
    int quantity_open = 0;
    bool has_parenteses = false;
    while (line != 0 || column != -1) {
        if (column == -1) {
            line -= 1;
            column = file_content[line].size();
            column -= 1;
            continue;
        }

        auto c = file_content[line][column];
        if (!mask_valid[line][column]) {
            column--;
            continue;
        }
        if (c == ')') {
            quantity_open++;
            has_parenteses = true;
            column--;
            continue;
        }
        if (c == '(') {
            quantity_open--;
            has_parenteses = true;
            column--;
            continue;
        }
        if (Utils::is_special_char(c) || quantity_open != 0) {
            column--;
            continue;
        }
        break;
    }
    assert(!(line == 0 && column == -1) && "code does not compile, bad formation of parenteses ()");
    return has_parenteses;
}

// extract function_name, declaration start line and header content
tuple<string, int, vector<string>> FunctionBreakerC::extract_header_related_information(int start_line, int start_column) {
    int line = start_line;
    int column = start_column - 1;

    bool has_parenteses = move_pointer_until_character_outside_parenteses(line, column);

    string file_name = "";
    while (column != -1) {
        auto c = file_content[line][column];
        if (Utils::is_special_char(c)) {
            break;
        }
        file_name += c;
        column--;
    }
    reverse(file_name.begin(), file_name.end());

    move_pointer_until_character_outside_parenteses(line, column);

    while (column != -1 || !Utils::is_special_char(file_content[line][column])) {
        column--;
    }
    column++;

    vector<string> header_content;
    if (start_column == 0) {
        header_content = build_function_content(line, column, start_line - 1, (int)file_content[start_line - 1].size() - 1);
    } else {
        header_content = build_function_content(line, column, start_line, start_column - 1);
    }

    if (!ALLOW_STRUCTS && !has_parenteses) {
        return {"", -1, header_content};
    }
    return {file_name, line, header_content};
}

bool FunctionBreakerC::is_body_function_empty(int start_number_line, int start_column, int end_number_line, int end_column) {
    vector<string> function_content = build_function_content(start_number_line, start_column, end_number_line, end_column);
    int count_not_empty_char = 0;
    for (auto line : function_content) {
        for (auto c : line) {
            if (!Utils::is_empty_char(c)) {
                count_not_empty_char++;
            }
        }
    }
    bool is_empty = count_not_empty_char <= 2;
    return is_empty;
}

void FunctionBreakerC::process_function(int start_number_line,
    int start_column,
    int end_number_line,
    int end_column,
    string relative_path) {
    string first_line = file_content[start_number_line];
    auto [function_name, line_declaration, header_content] = extract_header_related_information(start_number_line, start_column);
    if (function_name.empty()) {
        return;
    }
    if (IGNORE_EMPTY_FUNCTIONS) {
        if (is_body_function_empty(start_number_line, start_column, end_number_line, end_column)) {
            return;
        }
    }
    vector<string> function_content = build_function_content(start_number_line, start_column, end_number_line, end_column);

    create_source_file(start_number_line, end_number_line, relative_path, function_name, function_content);
    create_header_file(relative_path, function_name, header_content);
    create_info_file(line_declaration, start_number_line, end_number_line, relative_path, function_name);
}

string FunctionBreakerC::file_path_from_folder_path(string file_path, string folder_path) {
    string ret = "";
    for (size_t i = folder_path.size(); i < file_path.size(); i++) {
        ret += file_path[i];
    }
    return ret;
}

void FunctionBreakerC::file_breaker_c(string file_path, string folder_path) {
    string relative_path = file_path_from_folder_path(file_path, folder_path);
    file_content = Utils::read_file_generic(file_path);
    mask_valid = build_mask_valid_code();

    set<array<int, 4>> start_end_of_functions = find_start_end_of_brackets_of_given_depth();
    for (auto [start_line, start_column, end_line, end_column] : start_end_of_functions) {
        process_function(start_line, start_column, end_line, end_column, relative_path);
    }
}

FunctionBreakerC::FunctionBreakerC(string file_path, string folder_path) {
    file_breaker_c(file_path, folder_path);
}
