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

    int dummy = 1;
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

    // Close all connections
    connections.clear();

    if (shutdown(server_fd, SHUT_RDWR) == -1)
    {
        std::cout << "Failed to close server file descriptor: " << strerror(errno) << std::endl;
    }

    accept_connections_thread.join();

    // Can be free'd earlier?
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
            std::lock_guard<std::mutex> guard(connections_mutex);
            connections.emplace_back(std::make_unique<ClientConnection>(client_fd));
        }
    }
}

void ServerCore::handle_connections()
{
    while(1)
    {
        std::lock_guard<std::mutex> guard(connections_mutex);

        for (auto it = connections.begin(); it != connections.end();)
        {
            std::unique_ptr<ClientConnection>& client = *it;
            if (client->client_fd == -1)
            {
                it = connections.erase(it);
            }
            else
            {
                it++;
            }
        }
    }
}
