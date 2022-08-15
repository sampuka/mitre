#ifndef SRC_UTIL_HPP_
#define SRC_UTIL_HPP_

#include <string>
#include <vector>

std::vector<std::string> split_string(const std::string& str, const std::string& delim);

std::vector<std::string> split_crlf(const std::string& str);

#endif
