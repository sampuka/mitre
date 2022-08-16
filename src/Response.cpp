#include "Response.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

const std::string crlf = "\r\n";

Response::Response(Request request)
{
    std::string document_path = request.requested_document;

    if (document_path == "/")
    {
        document_path = "/index.html";
    }

    document_path = "www" + document_path;

    std::string response_body;

    std::ifstream document_file(document_path);

    bool file_failure = !document_file;

    if (file_failure)
    {
        response_body = "<html><head><title>o3o Not Found</title></head><body>o3o Not Found</body></html>";
    }
    else
    {
        std::stringstream buffer;

        buffer << document_file.rdbuf();

        response_body = buffer.str();
    }

    std::string status_line = "HTTP/1.1";

    if (file_failure)
    {
        status_line += " 404 Not Found";
    }
    else
    {
        status_line += " 200 OK";
    }

    std::vector<std::string> headers = {"Content-Length: " + std::to_string(response_body.size())};

    response_string = status_line + crlf;

    for (const std::string& header : headers)
    {
        response_string += header + crlf;
    }

    response_string += crlf + response_body + crlf;

    std::cout << response_string << std::endl;
}

Response::~Response()
{
}
