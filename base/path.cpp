
#include "path.hpp" 

std::vector<std::string> Path::split_path(const std::string& string_path){
	std::string aux = "";
	std::vector<std::string> ret;
	for(auto c : string_path){
		if(c == BAR){
			ret.push_back(aux);
			aux = "";
		}else{ 
			aux += c;
		}
	}
	if(!aux.empty()){
		ret.push_back(aux);
	}
	return ret;
}

size_t Path::find_position_start_relative_path() const{
	size_t sz = tokens.size();
	size_t ret = sz;
	for(size_t i = 0; i < sz-1; i++){
		if(tokens[i] == BASE_INIT_STRING && tokens[i+1] == SOURCE_STRING){
			ret = i+2;
		}
	}
	assert(ret != sz && "PATH NOT VALID FOR THE TOOL");
	return ret;
}

bool Path::is_empty() const{
	return tokens.empty();
}

Path::Path(std::string string_path){
	tokens = split_path(string_path);
	position_start_relative_path = find_position_start_relative_path();
}

std::string Path::build_string_path(const std::vector<std::string>& path) const{
	std::string string_path;
	int sz = path.size();
	for(int i = 0; i < sz; i++){
		string_path += path[i];
		if(i != sz-1){
			string_path += BAR;
		}
	}
	return string_path;
}

std::string Path::build_base_path(const std::string& base) const{
	if(is_empty()){
		return "";
	}
	std::vector<std::string> path = tokens;
	int pos_change = position_start_relative_path-1;
	path[pos_change] = base;
	std::string string_path = build_string_path(path);
	return string_path;
}

std::string Path::build_source_path() const{
	return build_base_path(SOURCE_STRING);
}

std::string Path::build_header_path() const{
	return build_base_path(HEADER_STRING);
}

std::string Path::build_info_path() const{
	std::string ret = build_base_path(INFO_STRING);
	if(ret == ""){
		return "";
	}
	ret = remove_extension(ret);
	if(ret == ""){
		return "";
	}
	ret += JSON_EXTENSION;
	return ret;
}

std::vector<std::string> Path::get_tokens_from_relative_path() const{
	std::vector<std::string> token_relative_path = tokens;
	token_relative_path.pop_back();
	int to_remove = position_start_relative_path;
	reverse(token_relative_path.begin(),token_relative_path.end());
	for(int i = 0; i < to_remove; i++){
		token_relative_path.pop_back();
	}
	reverse(token_relative_path.begin(),token_relative_path.end());
	return token_relative_path;
}

std::string Path::remove_extension(std::string token) const{
	while(!token.empty()){
		auto c = token.back();
		token.pop_back();
		if(c == '.'){
			break;
		}
	}
	return token;
}

std::string Path::build_relative_path() const{
	if(is_empty()){
		return "";
	}
	std::vector<std::string> token_relative_path = get_tokens_from_relative_path();
	std::string string_path = build_string_path(token_relative_path);
	return string_path;
}

std::string Path::build_function_name() const{
	if(is_empty()){
		return "";
	}
	std::string function_name = tokens.back();
	function_name = remove_extension(function_name);
	return function_name;
}

std::vector<std::string> Path::get_common_folders(const Path& path) const{
	std::vector<std::string> tokens_relative_1 = get_tokens_from_relative_path();
	std::vector<std::string> tokens_relative_2 = path.get_tokens_from_relative_path();
	int minimum_size_tokens = std::min(tokens_relative_1.size(),tokens_relative_2.size());
	std::vector<std::string> common_folders;
	for(int i = 0; i < minimum_size_tokens; i++){
		auto token_1 = tokens_relative_1[i];
		auto token_2 = tokens_relative_2[i];
		if(token_1 == token_2){
			common_folders.push_back(token_1);
		}else{
			break;
		}
	}
	return common_folders;
}

bool Path::operator<(const Path &path) const{
	return tokens < path.tokens;
}

bool Path::contains_given_pattern(const std::string& pattern) const{
	std::string relative_path_plus_function_name = build_relative_path() + BAR + build_function_name();
	return relative_path_plus_function_name.find(pattern) != std::string::npos;
}
