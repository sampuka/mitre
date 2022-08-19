#include "Response.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const std::string crlf = "\r\n";

Response::Response(const Request& request_) : request(request_)
{
    if (request.request_type == "GET")
    {
        construct_get_response();
    }

    std::cout << response_string << std::endl;
}

Response::~Response()
{
}

void Response::construct_get_response()
{
    std::string document_path = request.requested_document;

    if (document_path == "/")
    {
        document_path = "/index.html";
    }

    document_path = "www" + document_path;

    std::string response_body;

    std::ifstream document_file(document_path);

    bool file_success = document_file.is_open();

    if (file_success)
    {
        std::stringstream buffer;

        buffer << document_file.rdbuf();

        response_body = buffer.str();
    }
    else
    {
        std::ifstream not_found_document("www/404.html");

        if (not_found_document.is_open())
        {
            std::stringstream buffer;

            buffer << not_found_document.rdbuf();

            response_body = buffer.str();
        }
        else
        {
            response_body = "<html><head><title>404 Not Found</title></head><body>404 404 Not Found Not Found</body></html>";
        }
    }

    std::string status_line = "HTTP/1.1 ";

    if (file_success)
    {
        status_line += "200 OK";
    }
    else
    {
        status_line += "404 Not Found";
    }

    std::vector<std::string> headers = {"Content-Length: " + std::to_string(response_body.size())};

    response_string = status_line + crlf;

    for (const std::string& header : headers)
    {
        response_string += header + crlf;
    }

    response_string += crlf + response_body + crlf;
}
