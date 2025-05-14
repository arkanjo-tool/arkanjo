// Contains the abstraction of a path to a function in the temporary code base.
// Useful for elements that does not need to, in fact, read the files in the 
// temporary codebase.

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
using namespace std;

class Config{
	string base_path = "tmp";

protected:
	static Config* config_;

	Config(){}

public:
	Config(Config &other) = delete;
	void operator=(const Config &) = delete;

	static Config *config();

	string getBasePath();
	void setTestConfig();
	void setDefaultConfig();
};

#endif
