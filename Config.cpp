#include <fstream>

#include "Config.h"

namespace {
	std::string rtrim(const std::string& str) {
		size_t last = str.find_last_not_of(" \t\n\r");
		return std::string::npos == last ? str : str.substr(0, last+1);
	}
	std::string ltrim(const std::string& str) {
		size_t first = str.find_first_not_of(" \t\n\r");
		return std::string::npos == first ? str : str.substr(first);
	}
}

Config::Config(const std::string& str) {
	std::ifstream infile(str.c_str());
	std::string curLine, key, value;
	while (std::getline(infile, curLine)) {
		std::stringstream ss;
		ss<<rtrim(curLine);
		ss>>key;
		std::getline(ss, value);
		value = ltrim(rtrim(value));
		map[key] = value;
		std::cout<<"Loaded Config: "<<key<<"="<<value<<std::endl;
	}
}
