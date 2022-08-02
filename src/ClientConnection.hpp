#ifndef CONNECTION_HPP_H_
#define CONNECTION_HPP_H_

#include <thread>

class ClientConnection
{
public:
    ClientConnection() = delete;
    ClientConnection(int client_fd_);
    ~ClientConnection();

    int client_fd = -1;

private:
    void connection_loop();
    std::thread connection_thread;

    bool shutdown_signal = false;
};

#endif
