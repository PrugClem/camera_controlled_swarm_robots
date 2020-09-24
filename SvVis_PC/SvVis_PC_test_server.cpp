#include "SvVis_PC.hpp"

int main()
{
    cppsock::socket listener, server;
    cppsock::tcp_server_setup(listener, nullptr, 10001, 1);
    std::cout << "Server listening to " << listener.getsockname().get_addr() << ", port " << listener.getsockname().get_port() << std::endl;
    listener.accept(server);
    listener.close();

    server.send("\x0a", 1, 0);
    server.send("abcdefghijklmnopqrstuvwxyz0123456789", 30, 0);
    server.send("\0", 1, 0);

    server.close();
}