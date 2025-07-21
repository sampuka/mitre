#include "ServerCore.hpp"

#include "Configuration.hpp"

#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <iostream>

ServerCore::ServerCore()
{
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Listen on this computer

    int getaddrinfo_status = getaddrinfo(nullptr, conf.port.c_str(), &hints, &server_info);

    if (getaddrinfo_status != 0)
    {
        std::printf("Failed to get address info: %s\n", gai_strerror(getaddrinfo_status));
        exit(1);
    }

    server_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (server_fd == -1)
    {
        std::printf("Failed to create socket: %s\n", strerror(errno));
        exit(1);
    }

    int dummy = 1;
    int setsockopt_status = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &dummy, sizeof dummy);

    if (setsockopt_status == -1)
    {
        std::printf("Failed to set socket options: %s\n", strerror(errno));
        exit(1);
    }

    std::printf("Failed to set socket options: %s...\n", conf.port.c_str());

    int bind_status = bind(server_fd, server_info->ai_addr, server_info->ai_addrlen);

    if (bind_status == -1)
    {
        std::printf("Failed to bind to port %s: %s\n", conf.port.c_str(), strerror(errno));
        exit(1);
    }

    std::printf("Accepting packages...");

    int listen_status = listen(server_fd, conf.listen_backlog_size);

    if (listen_status == -1)
    {
        std::printf("Failed to bind to listen on socket: %s\n", strerror(errno));
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

    if (close(server_fd) == -1)
    {
        std::printf("Failed to close server file descriptor: %s\n", strerror(errno));
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
                std::printf("Failed to accept a connection: %s\n", strerror(errno));
                continue;
            }
        }
        else
        {
            std::lock_guard<std::mutex> guard(connections_mutex);
            struct in_addr ip = reinterpret_cast<struct sockaddr_in*>(&client_addr)->sin_addr;
            connections.emplace_back(std::make_unique<ClientConnection>(client_fd, inet_ntoa(ip)));
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
