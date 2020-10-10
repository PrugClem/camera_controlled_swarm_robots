#ifndef	SvVIS_CORTEX_H
#define SvVIS_CORTEX_H

// RTOS API here: http://www.keil.com/pack/doc/CMSIS/RTOS2/html/index.html

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <cmsis_os2.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_usart.h>

#include "config.h"

typedef struct SvVis3_t
{
    USART_TypeDef *port;
    osThreadId_t thread_usart, thread_recv, thread_send;
    osMessageQueueId_t queue_usart, queue_recv, queue_send;
} SvVis3_t;

typedef struct SvVis3_message_t
{
    int8_t channel;
    int8_t len;
    union SvVis3_message_data_t
    {
        int32_t i32;
        int16_t i16;
        float f;
        char raw[SvVIS3_DATA_MAX_LEN];
    } data;    
} SvVis3_message_t;

typedef struct SvVis3_data_t
{
    struct SvVis3_data_tuple_t
    {
        int32_t i32;
        int16_t i16;
        float f;
    } ch[SvVis3_CHANNEL_COUNT];
} SvVis3_data_t;

void USART_send_bytes(USART_TypeDef*, void *start, size_t len);
void USART_send_byte(USART_TypeDef*, uint8_t byte);

void init_usart(USART_TypeDef *usartn, uint32_t baud);
osStatus_t SvVis3_init(SvVis3_t* tar, USART_TypeDef* USARTx, uint32_t baud);
osStatus_t SvVis3_send_message(SvVis3_t*, SvVis3_message_t *message);
osStatus_t SvVis3_send_string(SvVis3_t*, const char *str);
osStatus_t SvVis3_send_I16(SvVis3_t*, SvVis3_channel_t channel, int16_t data);
osStatus_t SvVis3_send_I32(SvVis3_t*, SvVis3_channel_t channel, int32_t data);
osStatus_t SvVis3_send_float(SvVis3_t*, SvVis3_channel_t channel, float data);
osStatus_t SvVis3_recv_message(SvVis3_t*, SvVis3_message_t *msg_buf, uint32_t timeout);
void SvVis3_apply_msg(SvVis3_data_t *container, const SvVis3_message_t *msg);
bool SvVis3_message_is_string(const SvVis3_message_t *msg, char *optional_output);
void SvVis3_message_extract_string(const SvVis3_message_t *msg, char *output);

// thread functions, do not use
__NO_RETURN void usart_thread(void *arg);
__NO_RETURN void recv_thread(void *arg);
__NO_RETURN void send_thread(void *arg);

#endif // SvVIS_CORTEX_H
