#ifndef SERVERCORE_HPP_
#define SERVERCORE_HPP_

#include "ClientConnection.hpp"

#include <memory>
#include <mutex>
#include <thread>
#include <vector>

class ServerCore
{
public:
    ServerCore();
    ~ServerCore();

private:
    void accept_connections();
    std::thread accept_connections_thread;

    void handle_connections();

    bool shutdown_signal = false;

    struct addrinfo* server_info = nullptr;
    const char* listen_port = "80";
    const int listen_backlog_size = 10;

    int server_fd = -1;

    std::vector<std::unique_ptr<ClientConnection>> connections;

    std::mutex connections_mutex;
};

#endif
