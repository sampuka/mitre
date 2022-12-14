#include "Configuration.hpp"

#include "util.hpp"

#include <fstream>
#include <sstream>

Configuration global_conf("mitre.conf");

Configuration::Configuration(std::string conf_filepath)
{
    std::ifstream conf_file(conf_filepath);

    std::stringstream buffer;

    buffer << conf_file.rdbuf();

    std::string conf_string = buffer.str();

    std::vector<std::string> conf_lines = split_string(conf_string, "\n");

    for (const std::string& conf_line : conf_lines)
    {
        if (conf_line.size() == 0 || conf_line.at(0) == '#')
        {
            continue;
        }

        std::vector<std::string> conf_words = split_string(conf_line, " ");

        if (conf_words.at(0) == "webroot" && conf_words.size() == 2)
        {
            webroot = conf_words.at(1);
        }
        else if (conf_words.at(0) == "auxroot" && conf_words.size() == 2)
        {
            auxroot = conf_words.at(1);
        }
        else if (conf_words.at(0) == "redirect" && conf_words.size() == 3)
        {
            redirects.push_back({conf_words.at(1), conf_words.at(2)});
        }
    }
}

Configuration::~Configuration()
{
}

std::string Configuration::get_replacement(std::string key)
{
    for (const std::pair<std::string, std::string>& redirect : redirects)
    {
        if (redirect.first == key)
        {
            return redirect.second;
        }
    }

    return "";
}
