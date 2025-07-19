#include "Response.hpp"

#include "Configuration.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

const std::string crlf = "\r\n";

Response::Response(const Request& request_) : request(request_)
{
    if (request.request_type == "GET")
    {
        construct_get_response();
    }
}

void Response::construct_get_response()
{
    std::string document_path = request.requested_document;

    // TODO: VULNERABILITY

    if (document_path == "/")
    {
        document_path = "/index.html";
    }

    document_path = document_path;

    std::string response_body;

    headers.clear();

    std::ifstream document_file(conf.webroot + document_path);

    bool file_success = document_file.is_open();

    if (file_success)
    {
        std::stringstream buffer;

        buffer << document_file.rdbuf();

        response_body = buffer.str();

        if (document_path.ends_with(".html"))
        {
            content_type = "text/html";
        }
        else if (document_path.ends_with(".ico"))
        {
            content_type = "image/x-icon";
        }
        else
        {
            std::cout << "Serving content of unknown type!" << std::endl;
        }
    }
    else
    {
        document_path = "/404.html";

        std::ifstream not_found_document(conf.webroot + document_path);

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

        content_type = "text/html";
    }

    if (content_type == "text/html")
    {
        replace_tokens(response_body);
    }

    headers.push_back("Content-Length: " + std::to_string(response_body.size()));
    headers.push_back("Content-Type: " + content_type);
    headers.push_back("Content-Location: " + document_path);

    status_line = "HTTP/1.1 ";

    if (file_success)
    {
        status_line += "200 OK";
    }
    else
    {
        status_line += "404 Not Found";
    }

    response_string = status_line + crlf;

    for (const std::string& header : headers)
    {
        response_string += header + crlf;
    }

    response_string += crlf + response_body + crlf;
}

void Response::print() const
{
    std::cout << status_line << '\n';

    for (const std::string& header : headers)
    {
        std::cout << header << '\n';
    }
}

void Response::replace_tokens(std::string& str)
{
    for (const std::pair<std::string, std::string>& redirect : conf.redirects)
    {
        std::string token = "<!-- " + redirect.first + " -->";

        size_t loc = str.find(token);

        std::stringstream buffer;

        std::ifstream token_file(conf.resources + redirect.second);

        buffer << token_file.rdbuf();

        std::string token_replacement = buffer.str();

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }

    {
        std::string token = "<!-- CLIENT_IP -->";

        size_t loc = str.find(token);

        std::string token_replacement = request.client_ip;

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }

    {
        std::string token = "<!-- CLIENT_USER_AGENT -->";

        size_t loc = str.find(token);

        std::string token_replacement = request.user_agent;

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }
}
