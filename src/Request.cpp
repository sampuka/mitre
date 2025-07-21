#include "Request.hpp"

#include "util.hpp"

#include <sstream>
#include <iostream>

Request::Request(std::string request_string)
{
    std::vector<std::string> header_fields = split_string(request_string, "\r\n");

    if (header_fields.size() == 0)
    {
        std::printf("Empty request!!!\n");
        return;
    }

    {
        const std::string request_line = header_fields.at(0);
        header_fields.erase(header_fields.begin());

        std::vector<std::string> request_line_words = split_string(request_line, " ");

        if (request_line_words.size() != 3)
        {
            std::string str = "Request line length not 3:\n";
            for (const std::string& word : request_line_words)
            {
                str += word + '\n';
            }
            std::printf("%s\n", str.c_str());
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
        else if (name == "Cf-Connecting-Ip")
        {
            connecting_ip = value;
        }
        else if (name == "Cf-Ipcity")
        {
            city = value;
        }
        else if (name == "Cf-Ipcontinent")
        {
            continent = value;
        }
        else if (name == "Cf-Ipcountry")
        {
            country = value;
        }
        else if (name == "Cf-Iplatitude")
        {
            latitude = value;
        }
        else if (name == "Cf-Iplongitude")
        {
            longitude = value;
        }
        else if (name == "Cf-Postal-Code")
        {
            postal_code = value;
        }
        else if (name == "Cf-Region")
        {
            region = value;
        }
        else if (name == "Cf-Timezone")
        {
            timezone = value;
        }
    }
}
