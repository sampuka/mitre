#include "ServerCore.hpp"

#include <cstdio>

int main(int argc, char** argv)
{
    (void) argc;
    (void) argv;

    std::setbuf(stdout, NULL);

    ServerCore server;

    return 0;
}
