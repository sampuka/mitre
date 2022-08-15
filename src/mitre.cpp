#include "ServerCore.hpp"

const char* bind_port = "80";
const int listen_backlog_size = 10;

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    ServerCore server;

    return 0;
}
