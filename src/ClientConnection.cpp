#include "ClientConnection.hpp"

#include "Request.hpp"
#include "Response.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <cstdio>
//#include <iostream>
#include <string>
#include <vector>

ClientConnection::ClientConnection(int client_fd_, std::string client_ip_)
{
    client_fd = client_fd_;
    client_ip = client_ip_;
    connection_thread = std::thread(&ClientConnection::connection_loop, this);
}

ClientConnection::~ClientConnection()
{
    shutdown_signal = true;

    close_socket();

    connection_thread.join();
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
            close_socket();
        }
        else if (bytes_received == -1)
        {
            std::printf("Error receiving bytes: %s\n", strerror(errno));
        }
        else
        {
            buffer[5000] = '\n';

            Request request(buffer);

            Response response(request);

            std::string response_string = response.response_string;

            ssize_t bytes_sent = send(client_fd, response_string.c_str(), response_string.size(), 0);

            std::printf("Received:\n%sResponse:\n%sResponse size: %u bytes_sent: %u\n\n", buffer, response.print().c_str(), response_string.size(), bytes_sent);
        }
    }
}

void ClientConnection::close_socket()
{
    if (client_fd != -1)
    {
        if (close(client_fd) == -1)
        {
            std::printf("Error closing client connection: %s\n", strerror(errno));
        }
        client_fd = -1;
    }
}
