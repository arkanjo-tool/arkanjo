#include <iostream>

#include "function.hpp"

void Function::read_content(){
	std::string source_path = path.build_source_path();
	content = Utils::read_file_generic(source_path);
}	

void Function::read_header(){
	std::string header_path = path.build_header_path();
	header = Utils::read_file_generic(header_path);
}
void Function::read_info(){
	std::string info_path = path.build_info_path();
	json info = Utils::read_json(info_path);
	line_declaration = info.value(LINE_DECLARATION_JSON,-1);
	start_number_line = info.value(START_NUMBER_LINE_JSON,-1);
	end_number_line = info.value(END_NUMBER_LINE_JSON,-1);
}

int Function::number_of_lines() const{
	return end_number_line-line_declaration+1;
}

Function::Function(const Path& _path){
	path = _path;
}

void Function::load() {
	if(path.is_empty()){
		return;
	}
	read_content();
	read_header();
	read_info();
}

std::array<int,3> Function::get_scope_function_in_file() const{
	return {line_declaration,start_number_line,end_number_line};
}

std::vector<std::string> Function::get_header() const{
	return header;
}

std::vector<std::string> Function::build_all_content(){
	
	std::vector<std::string> ret;
	//the last line of header should merge with the first line of content
	for(auto line : header){
		ret.push_back(line);
	}
	for(int i = 0; i < int(content.size()); i++){
		auto line = content[i];
		if(i >= 1){
			ret.push_back(line);
		}else{
			ret.back() += line;
		}
	}
	return ret;
}

void Function::print_basic_info(){
	std::string function_message = FUNCTION_PREFIX_PRINT + path.build_function_name();
	std::string relative_message = RELATIVE_PATH_PRINT + path.build_relative_path();
	std::string start_message = LINE_DECLARATION_PRINT + std::to_string(line_declaration+1);
	std::string end_message = END_DECLARATION_PRINT + std::to_string(end_number_line+1);
	std::string number_message = NUMBER_LINE_PRINT + std::to_string(number_of_lines());
	
	std::cout << '\n';
	std::cout << Utils::LIMITER_PRINT << '\n';
	std::cout << Utils::format_colored_message(function_message, Utils::BRIGHT_YELLOW) << '\n';
	std::cout << Utils::format_colored_message(relative_message, Utils::GREEN) << '\n';
	std::cout << Utils::format_colored_message(start_message, Utils::WHITE) << '\n';
	std::cout << Utils::format_colored_message(end_message, Utils::WHITE) << '\n';
	std::cout << Utils::format_colored_message(number_message, Utils::WHITE) << '\n';
	std::cout << Utils::LIMITER_PRINT << '\n';
	std::cout << '\n';
}
