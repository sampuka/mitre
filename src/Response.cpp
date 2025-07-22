#include "Response.hpp"

#include "Configuration.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <filesystem>

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

    if (document_path == "/")
    {
        document_path = "/index.html";
    }

    std::string response_body;

    status_line = "HTTP/1.1 200 OK";

    headers.clear();

    std::filesystem::path base_path = conf.webroot;
    std::filesystem::path requested_path(conf.webroot + document_path);

    std::error_code ec;
    std::filesystem::path resolved_path = std::filesystem::weakly_canonical(requested_path, ec);

    
    if (ec || resolved_path.string().find(base_path) != 0)
    {
        status_line = "HTTP/1.1 403 Forbidden";

        document_path = "/403.html";
    }
    else if (!std::filesystem::exists(resolved_path) || !std::filesystem::is_regular_file(resolved_path))
    {
        status_line = "HTTP/1.1 404 Not Found";

        document_path = "/404.html";
    }

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
        else if (document_path.ends_with(".css"))
        {
            content_type = "text/css";
        }
        else if (document_path.ends_with(".ico"))
        {
            content_type = "image/x-icon";
        }
        else if (document_path.ends_with(".svg"))
        {
            content_type = "image/svg+xml";
        }
        else if (document_path.ends_with(".png"))
        {
            content_type = "image/png";
        }
        else
        {
            std::printf("Serving content of unknown type! (%s)\n", document_path.c_str());
        }
    }
    else
    {
        status_line = "HTTP/1.1 404 Not Found";
        document_path = "/404.html";
        response_body = "<html><head><title>404 Not Found</title></head><body>404 404 Not Found Not Found</body></html>";
        content_type = "text/html";
    }

    if (content_type == "text/html")
    {
        replace_tokens(response_body);
    }

    headers.push_back("Content-Length: " + std::to_string(response_body.size()));
    headers.push_back("Content-Type: " + content_type);
    headers.push_back("Content-Location: " + document_path);

    response_string = status_line + crlf;

    for (const std::string& header : headers)
    {
        response_string += header + crlf;
    }

    response_string += crlf + response_body + crlf;
}

std::string Response::print() const
{
    std::string str = status_line + '\n';

    for (const std::string& header : headers)
    {
        str += header + '\n';
    }

    return str;
}

void Response::replace_tokens(std::string& str)
{
    for (const std::pair<std::string, std::string>& redirect : conf.redirects)
    {
        std::string token = "<!-- " + redirect.first + " -->";

        size_t loc = str.find(token);

        std::stringstream buffer;

        std::ifstream token_file(conf.root + redirect.second);

        buffer << token_file.rdbuf();

        std::string token_replacement = buffer.str();

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

    {
        std::string token = "<!-- CLIENT_IP -->";

        size_t loc = str.find(token);

        std::string token_replacement = request.connecting_ip;

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }

    {
        std::string token = "<!-- CLIENT_CITY -->";

        size_t loc = str.find(token);

        std::string token_replacement = request.city;

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }

    {
        std::string token = "<!-- CLIENT_COUNTRY -->";

        size_t loc = str.find(token);

        std::string token_replacement = request.country;

        while (loc != std::string::npos)
        {
            str.replace(loc, token.size(), token_replacement);

            loc = str.find(token);
        }
    }
}
