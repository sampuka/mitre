#include "Request.hpp"

#include "util.hpp"

#include <sstream>
#include <iostream>

Request::Request(std::string request_string)
{
    std::vector<std::string> header_fields = split_string(request_string, "\r\n");

    if (header_fields.size() == 0)
    {
        std::cout << "Empty request!!!" << std::endl;
        return;
    }

    {
        const std::string request_line = header_fields.at(0);
        header_fields.erase(header_fields.begin());

        std::vector<std::string> request_line_words = split_string(request_line, " ");

        if (request_line_words.size() != 3)
        {
            std::cout << "Request line length not 3:\n";
            for (const std::string& word : request_line_words)
            {
                std::cout << word << '\n';
            }
            std::cout << std::endl;
            return;
        }

        request_type = request_line_words.at(0);
        requested_document = request_line_words.at(1);
        http_ver = request_line_words.at(2);
    }

    for (const std::string& header_field : header_fields)
    {
        if (header_field.size() == 0)
        {
            break;
        }

        size_t delim = header_field.find(": ");

        std::string name = header_field.substr(0, delim);

        std::string value = header_field.substr(delim + 2, header_field.size());

        std::cout << name << " = " << value << '\n';

        if (name == "Host")
        {
            host = value;
        }
        else if (name == "User-Agent")
        {
            user_agent = value;
        }
        else if (name == "Accept")
        {
            accept_types = value;
        }
        else if (name == "Accept-Language")
        {
            accept_langs = value;
        }
        else if (name == "Connection")
        {
            connection_type = value;
        }
    }

    std::cout << std::endl;
}

Request::~Request()
{
}
