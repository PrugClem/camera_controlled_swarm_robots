#include "SvVis_PC.hpp"
#include <thread>

void lo2cl(cppsock::socket *lo, cppsock::socket *cl)
{
    cppsock::socket listener_lo;
    cppsock::tcp_server_setup(listener_lo, "localhost", 10000, 1);
    listener_lo.accept(*lo);
    listener_lo.close();

    char buf[256];
    ssize_t len;
    while ( (len = lo->recv(buf, sizeof(buf), 0)) > 0 )
    {
        cl->send(buf, len, 0);
    }
    cl->close();
}
void cl2lo(cppsock::socket *lo, cppsock::socket *cl)
{
    cppsock::socket listener_cl;
    cppsock::tcp_server_setup(listener_cl, nullptr, 10001, 1);
    listener_cl.accept(*cl);
    listener_cl.close();

    char buf[256];
    ssize_t len;
    while ( (len = cl->recv(buf, sizeof(buf), 0)) > 0 )
    {
        lo->send(buf, len, 0);
    }
    lo->close();
}

int main()
{
    cppsock::socket socket_lo, socket_cl;
    SvVis sock_lo;
    std::thread l2c(lo2cl, &socket_lo, &socket_cl),
                c2l(cl2lo, &socket_lo, &socket_cl);
    std::cout << "set up, connecting to loopback" << std::endl;
    sock_lo.open("localhost", 10000);
    if(sock_lo.is_open())
    {
        std::cout << "server ready" << std::endl;
        SvVis3_message_t msg;
        sock_lo.recv_msg(msg);
        std::cout << msg.data.raw << std::endl;
        sock_lo.send_string("hello there");
        sock_lo.send_i16(0, 10);
        sock_lo.send_i32(1, 10);
        sock_lo.send_float(2, 10);
        sock_lo.send_string("end of communication");
        sock_lo.close();
    }
    std::cout << "stopping" << std::endl;
    l2c.join();
    c2l.join();
    return 0;
}