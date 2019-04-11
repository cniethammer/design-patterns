#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <sstream>
#include <vector>

namespace string_utils {

template <typename T>
static std::string join(const std::vector<T>& vec, const std::string delimiter) {
	std::stringstream ss;
	auto first = begin(vec), last = end(vec);
	if (first != last) {
		while (true) {
			ss << *first++;
			if (first == last)
				break;
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

template <typename T>
static std::vector<T> split(const std::string& str, const std::string delimiter = " ") {
	std::vector<T> split_vec;
	size_t pos = 0;
	size_t end = str.find(delimiter);
	while(end != std::string::npos) {
		split_vec.push_back(string_to_type<T>(str.substr(pos, end - pos)));
		pos = end + delimiter.length();
		end = str.find(delimiter, pos);
	}
	split_vec.push_back(string_to_type<T>(str.substr(pos)));
	return split_vec;
}

}

#endif
