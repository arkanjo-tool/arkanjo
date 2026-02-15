#include "function_breaker_java.hpp"

set<array<int, 3>> FunctionBreakerJava::find_start_end_and_depth_of_brackets(vector<string> brackets_content) {
    set<array<int, 3>> start_ends;
    int open_brackets = 0;

    vector<int> not_processed_open_brackets;
    auto process_open = [&](int line_number) {
        open_brackets++;
        not_processed_open_brackets.push_back(line_number);
    };
    auto process_close = [&](int line_number) {
        open_brackets--;
        if (open_brackets <= -1) {
            open_brackets = 0;
        } else {
            int matched_open_position = not_processed_open_brackets.back();
            not_processed_open_brackets.pop_back();
            int depth_of_open = not_processed_open_brackets.size();
            start_ends.insert({matched_open_position, line_number, depth_of_open});
        }
    };

    for (size_t i = 0; i < brackets_content.size(); i++) {
        auto line = brackets_content[i];
        for (auto c : line) {
            if (c == '{') {
                process_open(i);
            }
            if (c == '}') {
                process_close(i);
            }
        }
    }
    return start_ends;
}

set<pair<int, int>> FunctionBreakerJava::find_start_end_of_brackets_of_given_depth(vector<string> brackets_content, int depth) {
    set<pair<int, int>> ret;
    set<array<int, 3>> bracket_pairs = find_start_end_and_depth_of_brackets(brackets_content);
    for (auto [start, end, dep] : bracket_pairs) {
        if (dep == depth) {
            ret.insert({start, end});
        }
    }
    return ret;
}

int FunctionBreakerJava::find_position_first_open_bracket(string s) {
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c == '{') {
            return i;
        }
    }
    return -1;
}

string FunctionBreakerJava::extract_last_token_of_string(string s) {
    vector<string> tokens;
    string cur_token = "";
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (Utils::is_empty_char(c) || Utils::is_special_char(c)) {
            if (!cur_token.empty()) {
                tokens.push_back(cur_token);
            }
            cur_token = "";
        } else {
            cur_token += c;
        }
    }
    if (!cur_token.empty()) {
        tokens.push_back(cur_token);
    }

    if (tokens.empty()) {
        return "";
    }
    return tokens.back();
}

Line_content FunctionBreakerJava::build_line_code(int line_number, string content) {
    Line_content ret;
    ret.line_number = line_number;
    ret.content = content;
    return ret;
}

vector<Line_content> FunctionBreakerJava::get_lines_before_body_function(const vector<string>& file_content, int line_start_body_function, int pos_bracket) {
    vector<Line_content> ret;
    Line_content line_bracket = build_line_code(line_start_body_function, file_content[line_start_body_function]);
    // remove everything after {
    while (int(line_bracket.content.size()) > pos_bracket) {
        line_bracket.content.pop_back();
    }
    ret.push_back(line_bracket);

    int until = max(0, line_start_body_function - NUMBER_OF_LINES_BEFORE_FOR_FUNCTION_NAME);
    for (int i = line_start_body_function - 1; i >= until; i--) {
        ret.push_back(build_line_code(i, file_content[i]));
    }
    reverse(ret.begin(), ret.end());

    // remove empty lines/empty characters at the end of lines
    while (!ret.empty()) {
        if (ret.back().content.empty()) {
            ret.pop_back();
            continue;
        }
        if (Utils::is_empty_char(ret.back().content.back())) {
            ret.back().content.pop_back();
            continue;
        }
        break;
    }
    return ret;
}

vector<Line_content> FunctionBreakerJava::remove_parenteses_at_the_end_of_the_scope(vector<Line_content> code) {
    if (code.empty() || code.back().content.back() != ')') {
        return code;
    }
    int count_close_parenteses = 0;

    while (!code.empty()) {
        string content = code.back().content;
        while (!content.empty()) {
            if (content.back() == ')') {
                count_close_parenteses++;
            }
            if (content.back() == '(') {
                count_close_parenteses--;
            }
            content.pop_back();
            if (count_close_parenteses == 0) {
                break;
            }
        }
        code.back().content = content;
        if (count_close_parenteses == 0) {
            break;
        }
        code.pop_back();
    }
    return code;
}

vector<Line_content> FunctionBreakerJava::remove_content_until_find_parenteses_at_the_end(vector<Line_content> code) {
    while (!code.empty()) {
        string content = code.back().content;
        while (!content.empty()) {
            if (content.back() == ')') {
                break;
            } else {
                content.pop_back();
            }
        }
        code.back().content = content;
        if (!content.empty()) {
            break;
        }
        code.pop_back();
    }
    return code;
}

vector<Line_content> FunctionBreakerJava::remove_parameters_of_declaration_c(vector<Line_content> code) {
    if (!ALLOW_STRUCTS) {
        auto ret = remove_content_until_find_parenteses_at_the_end(code);
        return remove_parenteses_at_the_end_of_the_scope(ret);
    }
    return remove_parenteses_at_the_end_of_the_scope(code);
}

vector<Line_content> FunctionBreakerJava::remove_parameters_of_declaration_java(vector<Line_content> code) {
    auto ret = remove_content_until_find_parenteses_at_the_end(code);
    return remove_parenteses_at_the_end_of_the_scope(ret);
}

vector<Line_content> FunctionBreakerJava::remove_parameters_of_declaration(vector<Line_content> code, PROGRAMMING_LANGUAGE programming_language) {
    if (programming_language == C) {
        return remove_parameters_of_declaration_c(code);
    }
    if (programming_language == JAVA) {
        return remove_parameters_of_declaration_java(code);
    }
    return code;
}

pair<string, int> FunctionBreakerJava::extract_function_name_and_line_from_declaration(const vector<string>& file_content, int line_start_body_function, PROGRAMMING_LANGUAGE programming_language) {
    int pos = find_position_first_open_bracket(file_content[line_start_body_function]);
    vector<Line_content> code_before_bracket = get_lines_before_body_function(file_content, line_start_body_function, pos);
    vector<Line_content> code = remove_parameters_of_declaration(code_before_bracket, programming_language);
    if (code.empty()) {
        return make_pair("", -1);
    }
    string ret = extract_last_token_of_string(code.back().content);
    return {ret, code.back().line_number};
}

vector<string> FunctionBreakerJava::build_function_content(int start_number_line, int end_number_line, const vector<string>& file_content) {
    string first_line = file_content[start_number_line];
    int to_remove = find_position_first_open_bracket(first_line);

    vector<string> function_content;
    reverse(first_line.begin(), first_line.end());
    for (int i = 0; i < to_remove; i++) {
        first_line.pop_back();
    }
    reverse(first_line.begin(), first_line.end());
    function_content.push_back(first_line);
    for (int i = start_number_line + 1; i <= end_number_line; i++) {
        function_content.push_back(file_content[i]);
    }
    return function_content;
}

vector<string> FunctionBreakerJava::build_header_content(int start_number_line, int line_declaration, string relative_path, string function_name, const vector<string>& file_content) {
    vector<string> function_content;
    for (int i = line_declaration; i < start_number_line; i++) {
        function_content.push_back(file_content[i]);
    }

    string first_line = file_content[start_number_line];
    int to_keep = find_position_first_open_bracket(first_line);
    while (int(first_line.size()) > to_keep) {
        first_line.pop_back();
    }
    function_content.push_back(first_line);
    return function_content;
}

bool FunctionBreakerJava::is_body_function_empty(int start_number_line, int end_number_line, const vector<string>& file_content) {
    vector<string> function_content = build_function_content(start_number_line, end_number_line, file_content);
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

void FunctionBreakerJava::process_function(int start_number_line, int end_number_line, string relative_path, const vector<string>& file_content, PROGRAMMING_LANGUAGE programming_language) {
    string first_line = file_content[start_number_line];
    auto [function_name, line_declaration] = extract_function_name_and_line_from_declaration(file_content, start_number_line, programming_language);
    if (function_name.empty()) {
        return;
    }
    if (IGNORE_EMPTY_FUNCTIONS) {
        if (is_body_function_empty(start_number_line, end_number_line, file_content)) {
            return;
        }
    }

    vector<string> function_content = build_function_content(start_number_line, end_number_line, file_content);
    vector<string> header_content = build_header_content(start_number_line, line_declaration, relative_path, function_name, file_content);

    create_source_file(start_number_line, end_number_line, relative_path, function_name, function_content);
    create_header_file(relative_path, function_name, header_content);
    create_info_file(line_declaration, start_number_line, end_number_line, relative_path, function_name);
}

string FunctionBreakerJava::file_path_from_folder_path(string file_path, string folder_path) {
    string ret = "";
    for (size_t i = folder_path.size(); i < file_path.size(); i++) {
        ret += file_path[i];
    }
    return ret;
}

void FunctionBreakerJava::file_breaker_java(string file_path, string folder_path) {
    string relative_path = file_path_from_folder_path(file_path, folder_path);
    vector<string> file_content = Utils::read_file_generic(file_path);
    set<pair<int, int>> start_end_of_functions = find_start_end_of_brackets_of_given_depth(file_content, JAVA_RELEVANT_DEPTH);

    for (auto [start_line, end_line] : start_end_of_functions) {
        process_function(start_line, end_line, relative_path, file_content, JAVA);
    }
}

FunctionBreakerJava::FunctionBreakerJava(string file_path, string folder_path) {
    file_breaker_java(file_path, folder_path);
}
