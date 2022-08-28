#ifndef SRC_REQUEST_HPP_
#define SRC_REQUEST_HPP_

#include <string>

class Request
{
public:
    Request() = delete;
    Request(std::string request_string, std::string client_ip_);
    ~Request();

    std::string request_type;
    std::string requested_document;
    std::string http_ver;
    std::string host;
    std::string user_agent;
    std::string accept_types;
    std::string accept_langs;
    std::string connection_type;

    std::string client_ip;
};

#endif

