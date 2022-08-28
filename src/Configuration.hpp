#ifndef SRC_CONFIGURATION_HPP_
#define SRC_CONFIGURATION_HPP_

#include <string>
#include <utility>
#include <vector>

class Configuration
{
public:
    Configuration() = delete;
    Configuration(std::string conf_filepath);
    ~Configuration();

    std::string get_replacement(std::string key);

    std::string webroot;
    std::string auxroot;

    std::vector<std::pair<std::string, std::string>> redirects;
};

extern Configuration global_conf;

#endif
