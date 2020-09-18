#ifndef SvVIS_PC_HPP_INCLUDED
#define SvVIS_PC_HPP_INCLUDED

#include "../cppsock/cppsock.hpp"
#include "config.hpp"
#include <iostream>

struct SvVis3_message_t
{
    SvVis3_channel_t channel;
    int8_t len;
    union data
    {
        int16_t i16;
        int32_t i32;
        float f;
        char raw[SvVIS3_DATA_MAX_LEN];
    } data;
    bool is_string() {return this->channel == SvVIS3_STRING_CHANNEL;}
};

struct SvVis3_data_t
{
    struct SvVis3_data_tuple_t
    {
        int16_t i16;
        int32_t i32;
        float f;
    };
    SvVis3_data_tuple_t ch[SvVis3_CHANNEL_COUNT];
    void apply_msg(SvVis3_message_t &msg);
};

class SvVis
{
    cppsock::socket sock;
public:
    bool open(const std::string &conv_ser, uint16_t conv_port);
    bool is_open(void);
    void close(void);
    
    void recv_msg(SvVis3_message_t &msbguf);

    void send_msg(SvVis3_message_t &msg);
    void send_string(const std::string &str);
    void send_i16(SvVis3_channel_t channel, int16_t data);
    void send_i32(SvVis3_channel_t channel, int32_t data);
    void send_float(SvVis3_channel_t channel, float data);
};

#endif // SvVIS_PC_HPP_INCLUDED
