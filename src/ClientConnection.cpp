#include "ClientConnection.hpp"

#include "Request.hpp"
#include "Response.hpp"

#include <sys/socket.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

ClientConnection::ClientConnection(int client_fd_)
{
    std::cout << "Creating client connection..." << std::endl;

    client_fd = client_fd_;
    connection_thread = std::thread(&ClientConnection::connection_loop, this);

    std::cout << "Created client connection" << std::endl;
}

ClientConnection::~ClientConnection()
{
    std::cout << "Closing client connection..." << std::endl;
    shutdown_signal = true;

    if (client_fd != -1)
    {
        if (shutdown(client_fd, SHUT_RDWR) == -1)
        {
            std::cout << "Error closing client connection: " << strerror(errno) << std::endl;
        }
        client_fd = -1;
    }

    connection_thread.join();

    std::cout << "Closed client connection" << std::endl;
}

void ClientConnection::connection_loop()
{
    char buffer[5001];
    struct sockaddr_storage src_addr;
    socklen_t addrlen = sizeof src_addr;

    while(!shutdown_signal)
    {
        addrlen = sizeof src_addr;

        ssize_t bytes_received = recvfrom(client_fd, buffer, 5000, 0, reinterpret_cast<struct sockaddr*>(&src_addr), &addrlen);

        if (bytes_received == 0)
        {
            // Thread shutdown orderly
            shutdown_signal = true;
            client_fd = -1;
        }
        else if (bytes_received == -1)
        {
            std::cout << "Error receiving bytes: " << strerror(errno) << std::endl;
        }
        else
        {
            buffer[5000] = '\n';

            Request request(buffer);

            Response response_(request);

            const std::string crlf = "\r\n";

            std::string status_line = "HTTP/1.1 200 OK";

            std::string message_body = "<html>\r\n<head>\r\n</head>\r\n<body>\r\nHi\r\n</body></html>";

            std::vector<std::string> headers = {"Content-Length: " + std::to_string(message_body.size())};

            std::string response = status_line + crlf;

            for (const std::string& header : headers)
            {
                response += header + crlf;
            }

            response += crlf + message_body + crlf;

            ssize_t bytes_sent = send(client_fd, response.c_str(), response.size(), 0);

//            std::cout << "Received:\n" << buffer << "\nReponse:\n" << response;
            std::cout << "Response size: " << response.size() << " bytes_sent: " << bytes_sent << std::endl;
        }
    }
}
