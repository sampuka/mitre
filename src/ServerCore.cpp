#include "ServerCore.hpp"

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

ServerCore::ServerCore()
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Listen on this computer

    int getaddrinfo_status = getaddrinfo(nullptr, "80", &hints, &server_info);

    if (getaddrinfo_status != 0)
    {
        std::cout << "Failed to get address info: " << gai_strerror(getaddrinfo_status) << std::endl;
        exit(1);
    }

    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (server_fd == -1)
    {
        std::cout << "Failed to create socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    int dummy;
    int setsockopt_status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof dummy);

    if (setsockopt_status == -1)
    {
        std::cout << "Failed to set socket options: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Binding to port " << listen_port << "..." << std::endl;

    int bind_status = bind(server_fd, server_info->ai_addr, server_info->ai_addrlen);

    if (bind_status == -1)
    {
        std::cout << "Failed to bind to port " << listen_port << ": " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Accepting packages..." << std::endl;

    int listen_status = listen(server_fd, listen_backlog_size);

    if (listen_status == -1)
    {
        std::cout << "Failed to bind to listen on socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    accept_connections_thread = std::thread(&ServerCore::accept_connections, this);

    handle_connections();
}

ServerCore::~ServerCore()
{
    shutdown_signal = true;

    if (shutdown(connection, SHUT_RDWR) == -1)
    {
        std::cout << "Failed to close their file descriptor: " << strerror(errno) << std::endl;
    }

    if (shutdown(server_fd, SHUT_RDWR) == -1)
    {
        std::cout << "Failed to close server file descriptor: " << strerror(errno) << std::endl;
    }

    accept_connections_thread.join();

    freeaddrinfo(server_info);
}

void ServerCore::accept_connections()
{
    while(!shutdown_signal)
    {
        struct sockaddr_storage client_addr;
        socklen_t addrlen = sizeof client_addr;

        int client_fd = accept(server_fd, reinterpret_cast<struct sockaddr*>(&client_addr), &addrlen);

        if ((client_fd == -1))
        {
            if (!shutdown_signal)
            {
                std::cout << "Failed to accept a connection: " << strerror(errno) << std::endl;
                continue;
            }
        }
        else
        {
            // Mutex
            //connections.push_back(client_fd);
            connection = client_fd;
        }
    }
}

void ServerCore::handle_connections()
{
    char buffer[5001];
    struct sockaddr_storage src_addr;
    socklen_t addrlen = sizeof src_addr;

    while(1)
    {
        if (connection != -1)
        {
            addrlen = sizeof src_addr;

            int recv_bytes = recvfrom(connection, buffer, 5000, 0, reinterpret_cast<struct sockaddr*>(&src_addr), &addrlen);

            (void) recv_bytes;

            buffer[5000] = '\n';

            std::cout << "Received:\n" << buffer << std::endl;

            return;
        }
    }
}
