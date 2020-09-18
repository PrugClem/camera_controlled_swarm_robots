#include "SvVis_cortex.h"

uint8_t chid2len(uint8_t chid)
{
    if(chid == SvVIS3_STRING_CHANNEL) return SvVIS3_DATA_MAX_LEN;
    if((chid >= SvVIS3_FLOAT_CHANNEL_BASE) && (chid < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(float);
    if((chid >= SvVIS3_I16_CHANNEL_BASE) && (chid < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int16_t);
    if((chid >= SvVIS3_I32_CHANNEL_BASE) && (chid < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int32_t);
    return 0;
}

__NO_RETURN void usart_thread(void *arg)
{
    SvVis3_t *tar = arg;
    char input;
    for(;;)
    {
        while(USART_GetFlagStatus(tar->port, USART_FLAG_RXNE) == RESET) {osThreadYield();} // wait for character
        input = USART_ReceiveData(tar->port);
        osMessageQueuePut(tar->queue_usart, &input, NULL, osWaitForever);
    }
}
__NO_RETURN void recv_thread(void *arg)
{
    SvVis3_t *tar = arg;
    SvVis3_message_t msgbuf;
    char recvbuf;
    uint8_t maxlen;
    for(;;)
    {
        // init channel number
        msgbuf.len = 0;
        // get channel number
        osMessageQueueGet(tar->queue_usart, &recvbuf, NULL, osWaitForever);
        msgbuf.channel = recvbuf;
        maxlen = chid2len(msgbuf.channel);
        // get data
        if(msgbuf.channel != SvVIS3_STRING_CHANNEL)
        {
            while(msgbuf.len < maxlen) // not-string handling
            {
                osMessageQueueGet(tar->queue_usart, &recvbuf, NULL, osWaitForever);
                // push back recvbuf, if message is full, void rest of data
                if(msgbuf.len < SvVIS3_DATA_MAX_LEN) {msgbuf.data.raw[msgbuf.len++] = recvbuf;}
            }
        }
        else
        {
            while(recvbuf != '\0') // string handling
            {
                osMessageQueueGet(tar->queue_usart, &recvbuf, NULL, osWaitForever);
                // push back received character
                if(msgbuf.len < SvVIS3_DATA_MAX_LEN-1) {msgbuf.data.raw[msgbuf.len++] = recvbuf;}
                else                                   {msgbuf.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0;}
            }
        }
        osMessageQueuePut(tar->queue_recv, &msgbuf, NULL, 0);
    }
}
__NO_RETURN void send_thread(void *arg)
{
    SvVis3_t *tar = arg;
    SvVis3_message_t msg_buf;
    for(;;)
    {
        osMessageQueueGet(tar->queue_send, &msg_buf, NULL, osWaitForever);
        USART_send_byte(tar->port, msg_buf.channel); // send channel byte
        USART_send_bytes(tar->port, &msg_buf.data, msg_buf.len); // send data
    }
}
