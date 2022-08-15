#include "util.hpp"

std::vector<std::string> split_string(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;

    size_t start = 0;
    size_t end = str.find(delim, start);

    while (end != std::string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.size();
        end = str.find(delim, start);
    }

    if (start != str.size())
    {
        tokens.push_back(str.substr(start, str.size()));
    }

    return tokens;
}

std::vector<std::string> split_crlf(const std::string& str)
{
    return split_string(str, "\r\n");
}
