#ifndef SvVIS3_CORTEX_HPP_INCLUDED
#define SvVIS3_CORTEX_HPP_INCLUDED

// RTOS API here: http://www.keil.com/pack/doc/CMSIS/RTOS2/html/index.html

#include "config.hpp"
#include <string.h>

#include "misc.h"
#include "cmsis_os2.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"

#define nullptr NULL

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

class SvVis3_t
{
public:
    USART_TypeDef *port;
    //osThreadId_t thread_usart;
    osThreadId_t thread_recv, thread_send;
    osMessageQueueId_t queue_usart, queue_recv, queue_send;
    
    void init(USART_TypeDef *port, uint32_t baud);

    void send(SvVis3_message_t &msgbuf);
    void send(const char *str);
    void send(SvVis3_channel_t channel, int16_t data);
    void send(SvVis3_channel_t channel, int32_t data);
    void send(SvVis3_channel_t channel, float data);
    void recv(SvVis3_message_t &msgbuf);
};

extern SvVis3_t *_usart1_handler, *_usart2_handler, *_usart3_handler;

void init_usart(USART_TypeDef *usartn, uint32_t baud);
void USART_send_bytes(USART_TypeDef* port, void *start, size_t len);
void USART_send_byte(USART_TypeDef* port, uint8_t byte);
//__NO_RETURN void usart_thread(void *arg);
__NO_RETURN void recv_thread(void *arg);
__NO_RETURN void send_thread(void *arg);

#endif // SvVIS3_CORTEX_HPP_INCLUDED
