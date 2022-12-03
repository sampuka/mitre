#ifndef CONNECTION_HPP_H_
#define CONNECTION_HPP_H_

#include <string>
#include <thread>

class ClientConnection
{
public:
    ClientConnection() = delete;
    ClientConnection(int client_fd_, std::string client_ip_);
    ~ClientConnection();

    int client_fd = -1;
    std::string client_ip;

private:
    void connection_loop();
    std::thread connection_thread;

    void close_socket();

    bool shutdown_signal = false;
};

#endif
