#ifndef __CONFIG_H
#define __CONFIG_H

#include <string>
#include <map>
#include <sstream>
#include <iostream>

class Config {
	std::map<std::string, std::string> map;
public:
	Config(const std::string&);

	template <typename T>
	T get(const std::string& str) const {
		std::stringstream ss;
		std::map<std::string, std::string>::const_iterator it = map.find(str);
		if (it == map.end()) {
			std::cerr<<"Unable to find config item: "<<str<<std::endl;
			throw "Key not found.";
		};
		ss<<it->second;
		T ret;
		ss>>ret;
		return ret;
	}
};

#endif
