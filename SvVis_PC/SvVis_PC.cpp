#include "SvVis_PC.hpp"

void SvVis3_data_t::apply_msg(SvVis3_message_t &msg)
{
    if((msg.channel >= SvVIS3_I16_CHANNEL_BASE) && (msg.channel < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) // 11..19
    {
        this->ch[msg.channel-SvVIS3_I16_CHANNEL_BASE].i16 = msg.data.i16;
        this->ch[msg.channel-SvVIS3_I16_CHANNEL_BASE].i32 = msg.data.i16;
        this->ch[msg.channel-SvVIS3_I16_CHANNEL_BASE].f   = msg.data.i16;
    }
    else if ((msg.channel >= SvVIS3_I32_CHANNEL_BASE) && (msg.channel < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) // 1..9
    {
        this->ch[msg.channel-SvVIS3_I32_CHANNEL_BASE].i16 = msg.data.i32;
        this->ch[msg.channel-SvVIS3_I32_CHANNEL_BASE].i32 = msg.data.i32;
        this->ch[msg.channel-SvVIS3_I32_CHANNEL_BASE].f   = msg.data.i32;
    }
    else if ((msg.channel >= SvVIS3_FLOAT_CHANNEL_BASE) && (msg.channel < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) // 21..29
    {
        this->ch[msg.channel-SvVIS3_FLOAT_CHANNEL_BASE].i16 = msg.data.f;
        this->ch[msg.channel-SvVIS3_FLOAT_CHANNEL_BASE].i32 = msg.data.f;
        this->ch[msg.channel-SvVIS3_FLOAT_CHANNEL_BASE].f   = msg.data.f;
    }
}

bool SvVis::open(const std::string &conv_ser, uint16_t conv_port)
{
#if 0
    std::vector<cppsock::addressinfo> res;
    cppsock::socketaddr addr;

    if((cppsock::socketaddr::is(AF_INET, conv_ser)) || (cppsock::socketaddr::is(AF_INET6, conv_ser)))
    {
        addr.set(conv_ser, conv_port);
    }
    else
    {
        cppsock::resolve_tcp(conv_ser.c_str(), res, AF_INET);
        if(res.size() == 0) return false;
        addr = res.at(0);
        addr.set_port(conv_port);
    }

    //std::clog << "SvVis connect address: " << addr.get_family() << " " << addr.get_addr() << " " << addr.get_port() << std::endl;

    if( this->sock.init(addr.get_family(), SOCK_STREAM, IPPROTO_TCP) == SOCKET_ERROR )
        return false;
    if( this->sock.connect(addr) == SOCKET_ERROR )
    {
        //std::cerr << "error connecting to " << addr.get_addr() << " (port " << addr.get_port() << "): " << strerror(errno) << std::endl;
        this->sock.close();
        return false;
    }
    return true;
#else
    return cppsock::tcp_client_connect(this->sock, conv_ser.c_str(), conv_port) == 0;
#endif
}

bool SvVis::is_open(void)
{
    return this->sock.is_valid();
}

void SvVis::close(void)
{
    this->sock.close();
}

uint8_t chid2len(uint8_t chid)
{
    if(chid == SvVIS3_STRING_CHANNEL) return SvVIS3_DATA_MAX_LEN;
    if((chid >= SvVIS3_FLOAT_CHANNEL_BASE) && (chid < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(float);
    if((chid >= SvVIS3_I16_CHANNEL_BASE) && (chid < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int16_t);
    if((chid >= SvVIS3_I32_CHANNEL_BASE) && (chid < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int32_t);
    return 0;
}

void SvVis::recv_msg(SvVis3_message_t &msgbuf)
{
    char recvbuf;
    uint8_t maxlen;

    msgbuf.len = 0;
    this->sock.recv(&recvbuf, sizeof(recvbuf), 0); // get channel number
    msgbuf.channel = recvbuf;
    maxlen = chid2len(msgbuf.channel);
    if(msgbuf.channel != SvVIS3_STRING_CHANNEL)
    {
        while(msgbuf.len < maxlen) // not-string handling
        {
            this->sock.recv(&recvbuf, sizeof(recvbuf), 0);
            // push back recvbuf, if message is full, void rest of data
            if(msgbuf.len < SvVIS3_DATA_MAX_LEN) {msgbuf.data.raw[msgbuf.len++] = recvbuf;}
        }
    }
    else
    {
        while(recvbuf != '\0') // string handling
        {
            this->sock.recv(&recvbuf, sizeof(recvbuf), 0);
            // push back received character
            if(msgbuf.len < SvVIS3_DATA_MAX_LEN-2) {msgbuf.data.raw[msgbuf.len++] = recvbuf;} // character can be used
            else                                   {msgbuf.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0;} // no memory left, terminating character has to be put here
        }
        msgbuf.data.raw[msgbuf.len] = 0; // set ending character to 0
    }
}

void SvVis::send_msg(SvVis3_message_t &msg)
{
    this->sock.send(&msg.channel, sizeof(SvVis3_channel_t), 0);
    this->sock.send(msg.data.raw, msg.len, 0);
}

void SvVis::send_string(const std::string &str)
{
    SvVis3_message_t msg;
    msg.channel = SvVIS3_STRING_CHANNEL;
    msg.len = strlen(str.data())+1;
    strcpy(msg.data.raw, str.data());
    this->send_msg(msg);
}

void SvVis::send_i16(SvVis3_channel_t channel, int16_t data)
{
    SvVis3_message_t msg;
    msg.channel = SvVIS3_I16_CHANNEL_BASE + channel;
    msg.len = sizeof(data);
    msg.data.i16 = data;
    this->send_msg(msg);
}

void SvVis::send_i32(SvVis3_channel_t channel, int32_t data)
{
    SvVis3_message_t msg;
    msg.channel = SvVIS3_I32_CHANNEL_BASE + channel;
    msg.len = sizeof(data);
    msg.data.i32 = data;
    this->send_msg(msg);
}

void SvVis::send_float(SvVis3_channel_t channel, float data)
{
    SvVis3_message_t msg;
    msg.channel = SvVIS3_FLOAT_CHANNEL_BASE + channel;
    msg.len = sizeof(data);
    msg.data.f = data;
    this->send_msg(msg);
}
