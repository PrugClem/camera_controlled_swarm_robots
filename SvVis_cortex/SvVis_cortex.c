#include "SvVis_cortex.h"

void init_usart(USART_TypeDef *usartn)
{
    GPIO_InitTypeDef RX, TX;
    USART_InitTypeDef usart;
    USART_ClockInitTypeDef usart_clock;

    SystemCoreClockUpdate();

    TX.GPIO_Mode =  GPIO_Mode_AF_PP;
    RX.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    RX.GPIO_Speed = TX.GPIO_Speed = GPIO_Speed_50MHz;
    if(usartn == USART1)
    {
        // init USART1 RX(PA10) and TX(PA9)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOA, &TX);
    }
    else if(usartn == USART2)
    {
        // init USART2 RX(PA3) and TX(PA2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_3;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_2;
        GPIO_Init(GPIOA, &TX);
    }
    else if(usartn == USART3)
    {
        // init USART3 RX(PB11) and TX(PB10)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_11;
        GPIO_Init(GPIOB, &RX);
        TX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOB, &TX);
    }
    else
    {
        // no usart found, dun't run the code
        return;
    }

    USART_DeInit(usartn);

    usart_clock.USART_Clock = USART_Clock_Disable;
	usart_clock.USART_CPOL = USART_CPOL_Low;
	usart_clock.USART_CPHA = USART_CPHA_2Edge;
	usart_clock.USART_LastBit = USART_LastBit_Disable;
 
	USART_ClockInit(usartn, &usart_clock);

    usart.USART_BaudRate = USART_BAUD;
	usart.USART_WordLength = USART_WordLength_8b;	 
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(usartn, &usart);
	USART_Cmd(usartn, ENABLE);
}

void USART_send_bytes(USART_TypeDef* port, void *start, size_t len)
{
    for(size_t i=0; i<len; i++)
        USART_send_byte(port, ((char*)start)[i] );
}
void USART_send_byte(USART_TypeDef* port, uint8_t byte)
{
    while(USART_GetFlagStatus(port, USART_FLAG_TC) == RESET) {osThreadYield();}
    USART_SendData(port, byte);
}

osStatus_t SvVis3_init(SvVis3_t *tar, USART_TypeDef *usart)
{
    init_usart(usart);
    tar->port = usart;

    tar->queue_usart = osMessageQueueNew(SvVis3_USART_BACKLOG, sizeof(char), NULL);
    tar->queue_recv  = osMessageQueueNew(SvVIS3_RECV_BACKLOG, sizeof(SvVis3_message_t), NULL);
    tar->queue_send  = osMessageQueueNew(SvVIS3_SEND_BACKLOG, sizeof(SvVis3_message_t), NULL);

    tar->thread_usart = osThreadNew(usart_thread, tar, NULL);
    tar->thread_recv  = osThreadNew(recv_thread,  tar, NULL);
    tar->thread_send  = osThreadNew(send_thread,  tar, NULL);

    return osOK;
}

osStatus_t SvVis3_send_message(SvVis3_t *tar, SvVis3_message_t *message, uint32_t timeout)
{
    return osMessageQueuePut(tar->queue_send, message, NULL, timeout);
}

osStatus_t SvVis3_send_string(SvVis3_t *tar, const char *str, uint32_t timeout)
{
    size_t len = strlen(str)+1;
	bool too_long = (len >= SvVIS3_DATA_MAX_LEN);
    if(too_long) {len = SvVIS3_DATA_MAX_LEN;}
    SvVis3_message_t message = {.channel = SvVIS3_STRING_CHANNEL, .len = len};
    memcpy(message.data.raw, str, len);
	if(too_long) {message.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0; USART_send_byte(USART1, '#');} else {USART_send_byte(USART1, '!');}
    return SvVis3_send_message(tar, &message, timeout);
}
osStatus_t SvVis3_send_I16(SvVis3_t *tar, SvVis3_channel_t channel, int16_t data, uint32_t timeout)
{
    size_t len = sizeof(data);
    SvVis3_message_t message = {.channel = SvVIS3_I16_CHANNEL_BASE + channel, .len = len};
    message.data.i16 = data;
    return SvVis3_send_message(tar, &message, timeout);
}
osStatus_t SvVis3_send_I32(SvVis3_t *tar, SvVis3_channel_t channel, int32_t data, uint32_t timeout)
{
    size_t len = sizeof(data);
    SvVis3_message_t message = {.channel = SvVIS3_I32_CHANNEL_BASE + channel, .len = len};
    message.data.i32 = data;
    return SvVis3_send_message(tar, &message, timeout);
}
osStatus_t SvVis3_send_float(SvVis3_t *tar, SvVis3_channel_t channel, float data, uint32_t timeout)
{
    size_t len = sizeof(data);
    SvVis3_message_t message = {.channel = SvVIS3_FLOAT_CHANNEL_BASE + channel, .len = len};
    message.data.f = data;
    return SvVis3_send_message(tar, &message, timeout);
}
osStatus_t SvVis3_recv_message(SvVis3_t *tar, SvVis3_message_t *msg_buf, uint32_t timeout)
{
    return osMessageQueueGet(tar->queue_recv, msg_buf, NULL, timeout);
}
void SvVis3_apply_msg(SvVis3_data_t *container, const SvVis3_message_t *msg)
{
        if( (msg->channel >= SvVIS3_I16_CHANNEL_BASE) && (msg->channel < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 11..19
    {
        container->ch[msg->channel - SvVIS3_I16_CHANNEL_BASE].i16 = msg->data.i16;
        container->ch[msg->channel - SvVIS3_I16_CHANNEL_BASE].i32 = msg->data.i16;
        container->ch[msg->channel - SvVIS3_I16_CHANNEL_BASE].f   = msg->data.i16;
    }
    else if( (msg->channel >= SvVIS3_I32_CHANNEL_BASE) && (msg->channel < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 1..9
    {
        container->ch[msg->channel - SvVIS3_I32_CHANNEL_BASE].i16 = msg->data.i32;
        container->ch[msg->channel - SvVIS3_I32_CHANNEL_BASE].i32 = msg->data.i32;
        container->ch[msg->channel - SvVIS3_I32_CHANNEL_BASE].f   = msg->data.i32;
    }
    else if( (msg->channel >= SvVIS3_FLOAT_CHANNEL_BASE) && (msg->channel < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 21..29
    {
        container->ch[msg->channel - SvVIS3_FLOAT_CHANNEL_BASE].i16 = msg->data.f;
        container->ch[msg->channel - SvVIS3_FLOAT_CHANNEL_BASE].i32 = msg->data.f;
        container->ch[msg->channel - SvVIS3_FLOAT_CHANNEL_BASE].f   = msg->data.f;
    }
}
bool SvVis3_message_is_string(const SvVis3_message_t *msg, char *optional_output)
{
    if(msg->channel == SvVIS3_STRING_CHANNEL)
    {
        if(optional_output != NULL)
        {
            strcpy(optional_output, msg->data.raw);
        }
        return true;
    }
    return false;
}

void SvVis3_message_extract_string(const SvVis3_message_t *msg, char *output)
{
    if(msg->channel == SvVIS3_STRING_CHANNEL)
    {
        strcpy(output, msg->data.raw);
    }
}

