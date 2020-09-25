#include "SvVis_PC.hpp"
#include <thread>
#include <chrono>
#include <iostream>

int main()
{
    SvVis cortex;
    SvVis3_message_t msg;
    if(!cortex.open("localhost", 10001))
    {
        std::cout << "unable to connect to conversion server" << std::endl;
        exit(EXIT_FAILURE);
    }
    cortex.send_string("abcdefghijklmnopqrstuvwxyz0123456789");
    cortex.recv_msg(msg); // message received
    std::cout << msg.data.raw << std::endl;
    cortex.recv_msg(msg); // length i16
    std::cout << "(len[i16]:" << msg.data.i16 << ")" << std::endl;
    cortex.recv_msg(msg); // length 132
    std::cout << "(len[i32]:" << msg.data.i32 << ")" << std::endl;
    cortex.recv_msg(msg); // length float
    std::cout << "(len[float]:" << msg.data.f << ")" << std::endl;
    cortex.recv_msg(msg); // actual message
    std::cout << msg.data.raw << std::endl;
    cortex.close();
    return 0;
}
