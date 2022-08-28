#ifndef SRC_RESPONSE_HPP_
#define SRC_RESPONSE_HPP_

#include "Request.hpp"

#include <vector>

class Response
{
public:
    Response() = delete;
    Response(const Request& request);
    ~Response();

    void print() const;

    std::string status_line;
    std::string content_type;
    std::vector<std::string> headers;

    std::string response_string;

private:
    const Request& request;
    void construct_get_response();

    void replace_tokens(std::string& response_body);
};

#endif
