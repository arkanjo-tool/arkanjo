
#include "path.hpp" 

vector<string> Path::split_path(string string_path){
	string aux = "";
	vector<string> ret;
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

int Path::find_position_start_relative_path(){
	int sz = tokens.size();
	int ret = sz;
	for(int i = 0; i < sz-1; i++){
		if(tokens[i] == BASE_INIT_STRING && tokens[i+1] == SOURCE_STRING){
			ret = i+2;
		}
	}
	assert(ret != sz && "PATH NOT VALID FOR THE TOOL");
	return ret;
}

bool Path::is_empty(){
	return tokens.empty();
}

Path::Path(){
}

Path::Path(string string_path){
	tokens = split_path(string_path);
	position_start_relative_path = find_position_start_relative_path();
}

string Path::build_string_path(vector<string> path){
	string string_path;
	int sz = path.size();
	for(int i = 0; i < sz; i++){
		string_path += path[i];
		if(i != sz-1){
			string_path += BAR;
		}
	}
	return string_path;
}

string Path::build_base_path(string base){
	if(is_empty()){
		return "";
	}
	vector<string> path = tokens;
	int pos_change = position_start_relative_path-1;
	path[pos_change] = base;
	string string_path = build_string_path(path);
	return string_path;
}

string Path::build_source_path(){
	return build_base_path(SOURCE_STRING);
}

string Path::build_header_path(){
	return build_base_path(HEADER_STRING);
}

string Path::build_info_path(){
	string ret = build_base_path(INFO_STRING);
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

vector<string> Path::get_tokens_from_relative_path(){
	vector<string> token_relative_path = tokens;
	token_relative_path.pop_back();
	int to_remove = position_start_relative_path;
	reverse(token_relative_path.begin(),token_relative_path.end());
	for(int i = 0; i < to_remove; i++){
		token_relative_path.pop_back();
	}
	reverse(token_relative_path.begin(),token_relative_path.end());
	return token_relative_path;
}

string Path::remove_extension(string token){
	while(!token.empty()){
		auto c = token.back();
		token.pop_back();
		if(c == '.'){
			break;
		}
	}
	return token;
}

string Path::build_relative_path(){
	if(is_empty()){
		return "";
	}
	vector<string> token_relative_path = get_tokens_from_relative_path();
	string string_path = build_string_path(token_relative_path);
	return string_path;
}

string Path::build_function_name(){
	if(is_empty()){
		return "";
	}
	string function_name = tokens.back();
	function_name = remove_extension(function_name);
	return function_name;
}

vector<string> Path::get_common_folders(Path path){
	vector<string> tokens_relative_1 = get_tokens_from_relative_path();
	vector<string> tokens_relative_2 = path.get_tokens_from_relative_path();
	int minimum_size_tokens = min(tokens_relative_1.size(),tokens_relative_2.size());
	vector<string> common_folders;
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

bool Path::contains_given_pattern(string pattern){
	string relative_path_plus_function_name = build_relative_path() + BAR + build_function_name();
	return relative_path_plus_function_name.find(pattern) != string::npos;
}
