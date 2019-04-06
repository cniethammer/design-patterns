#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <sstream>
#include <vector>

namespace string_utils {

template <typename T>
static std::string join(const std::vector<T>& vec, const std::string delimiter) {
	std::stringstream ss;
	for(auto elem : vec) {
		ss << elem;
		if (elem != vec.back()) {
			ss << delimiter;
		}
	}
	return ss.str();
}

template <typename T>
T string_to_type(const std::string& str) {
	if(str == "") {return T();}
	std::stringstream str_stream(str);
	T value;
	str_stream >> value;
	return value;
}

}

#endif
