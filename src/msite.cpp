#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

const char* bind_port = "80";
const int listen_backlog_size = 10;

int main(int argc, char** argv)
{
    struct addrinfo* server_info = nullptr;

    struct addrinfo hints;
    std::memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;     // Listen on this computer

    int getaddrinfo_status = getaddrinfo(nullptr, "80", &hints, &server_info);

    if (getaddrinfo_status != 0)
    {
        std::cout << "Failed to getaddrinfo(): " << gai_strerror(getaddrinfo_status) << std::endl;
        exit(1);
    }

    int sockfd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (sockfd == -1)
    {
        std::cout << "Failed to create socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Binding to port " << bind_port << "..." << std::endl;

    int bind_status = bind(sockfd, server_info->ai_addr, server_info->ai_addrlen);

    if (bind_status == -1)
    {
        std::cout << "Failed to bind to port " << bind_port << ": " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Accepting packages..." << std::endl;

    int listen_status = listen(sockfd, listen_backlog_size);

    if (listen_status == -1)
    {
        std::cout << "Failed to bind to listen on socket: " << strerror(errno) << std::endl;
        exit(1);
    }

    struct sockaddr_storage their_addr;
    socklen_t addr_size = sizeof their_addr;

    int their_fd = accept(sockfd, reinterpret_cast<struct sockaddr*>(&their_addr), &addr_size);

    if (their_fd == -1)
    {
        std::cout << "Failed to accept a connection: " << strerror(errno) << std::endl;
        exit(1);
    }

    char buffer[5001];

    int recv_bytes = recv(their_fd, buffer, 5000, 0);

    buffer[5000] = '\n';

    std::cout << "Received:\n" << buffer << std::endl;

    if (close(their_fd) == -1)
    {
        std::cout << "Failed to close their file descriptor: " << strerror(errno) << std::endl;
    }

    if (close(sockfd) == -1)
    {
        std::cout << "Failed to close server file descriptor: " << strerror(errno) << std::endl;
    }

    freeaddrinfo(server_info);

    return 0;
}
