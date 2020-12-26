#include "SvVis_cortex.hpp"

uint8_t chid2len(uint8_t chid)
{
    if(chid == SvVIS3_STRING_CHANNEL) return SvVIS3_DATA_MAX_LEN;
    if((chid >= SvVIS3_FLOAT_CHANNEL_BASE) && (chid < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(float);
    if((chid >= SvVIS3_I16_CHANNEL_BASE) && (chid < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int16_t);
    if((chid >= SvVIS3_I32_CHANNEL_BASE) && (chid < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT)) return sizeof(int32_t);
    return 0;
}

extern "C" void USART1_IRQHandler(void)
{
    char input;
    input = USART_ReceiveData(USART1);
    osMessageQueuePut(_usart1_handler->queue_usart, &input, nullptr, 0);
    return;
}

extern "C" void USART2_IRQHandler(void)
{
    char input;
    input = USART_ReceiveData(USART2);
    osMessageQueuePut(_usart2_handler->queue_usart, &input, nullptr, 0);
    return;
}

// ===================================================================================================================================
// TODO: rewrite this function to handle WLAN module protocol
extern "C" void USART3_IRQHandler(void)
{
    char input;
    input = USART_ReceiveData(USART3);
    osMessageQueuePut(_usart3_handler->queue_usart, &input, nullptr, 0);
    return;
}

__NO_RETURN void recv_thread(void *arg)
{
    SvVis3_t *tar = (SvVis3_t*)arg;
    SvVis3_message_t msgbuf;
    char recvbuf;
    uint8_t maxlen;
    for(;;)
    {
        // init length number
        msgbuf.len = 0;
        osMessageQueueGet(tar->queue_usart, &recvbuf, nullptr, osWaitForever);
        msgbuf.channel = recvbuf;
        maxlen = chid2len(msgbuf.channel);
        // get data
        if(msgbuf.channel != SvVIS3_STRING_CHANNEL)
        {
            while(msgbuf.len < maxlen) // non-string handling
            {
                osMessageQueueGet(tar->queue_usart, &recvbuf, nullptr, osWaitForever);
                // push back recvbuf
                if(msgbuf.len < SvVIS3_DATA_MAX_LEN) {msgbuf.data.raw[msgbuf.len++] = recvbuf;}
            }
        }
        else
        {
            while(recvbuf != '\0')
            {
                osMessageQueueGet(tar->queue_usart, &recvbuf, nullptr, osWaitForever);
                // push back received character
                if(msgbuf.len < SvVIS3_DATA_MAX_LEN) {msgbuf.data.raw[msgbuf.len++] = recvbuf;} // write character from input data
                //{msgbuf.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0;} // write 0 at the end of the string
            }
            msgbuf.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0; // set ending character to 0
        }
        osMessageQueuePut(tar->queue_recv, &msgbuf, nullptr, osWaitForever);
    }
}
__NO_RETURN void send_thread(void *arg)
{
    SvVis3_t *tar = (SvVis3_t*)arg;
    SvVis3_message_t msgbuf;
    for(;;)
    {
        osMessageQueueGet(tar->queue_send, &msgbuf, nullptr, osWaitForever);
        USART_send_byte(tar->port, msgbuf.channel); // send channel byte
        USART_send_bytes(tar->port, &msgbuf.data, msgbuf.len); // send data
    }
}
