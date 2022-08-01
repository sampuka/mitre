#ifndef SERVERCORE_HPP_
#define SERVERCORE_HPP_

//#include "Connection.hpp"

#include <thread>

class ServerCore
{
public:
    ServerCore();
    ~ServerCore();

    void accept_connections();

    void handle_connections();

private:
    bool shutdown_signal = false;

    struct addrinfo* server_info = nullptr;
    const char* listen_port = "80";
    const int listen_backlog_size = 10;

    int server_fd = -1;

    //std::vector<Connection> connections;
    int connection = -1;

    std::thread accept_connections_thread;
};

#endif
