
#include "config.hpp" 

string Config::getBasePath(){
	return base_path;
}

void Config::setTestConfig(){
	base_path = "tests/e2e/current/tmp";
}

void Config::setDefaultConfig(){
	base_path = "tmp";
}

Config* Config::config_ = nullptr;

Config *Config::config(){
	if(config_ == nullptr){
		config_ = new Config();
	}
	return config_;
}
