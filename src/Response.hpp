#ifndef SRC_RESPONSE_HPP_
#define SRC_RESPONSE_HPP_

#include "Request.hpp"

class Response
{
public:
    Response() = delete;
    Response(const Request& request);
    ~Response();

    std::string response_string;

private:
    const Request& request;
    void construct_get_response();
};

#endif
