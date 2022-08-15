#ifndef SRC_RESPONSE_HPP_
#define SRC_RESPONSE_HPP_

#include "Request.hpp"

class Response
{
public:
    Response() = delete;
    Response(Request request);
    ~Response();
};

#endif
