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

    std::string get_replacement(std::string key);

    const std::string resources = "mitre/resources/";
    const std::string webroot = "mitre/www/";
    std::string port = "60053";
    const int listen_backlog_size = 10;

    std::vector<std::pair<std::string, std::string>> redirects;
};

inline Configuration conf("mitre/mitre.conf");

#endif
