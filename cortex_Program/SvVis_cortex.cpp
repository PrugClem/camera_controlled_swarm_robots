#include "SvVis_cortex.hpp"

SvVis3_t *_usart1_handler, *_usart2_handler, *_usart3_handler;

void init_usart(USART_TypeDef *usartn, uint32_t baud)
{

    GPIO_InitTypeDef RX, TX;
    USART_InitTypeDef usart;
    USART_ClockInitTypeDef usart_clock;
    NVIC_InitTypeDef nvic;

    SystemCoreClockUpdate();

    TX.GPIO_Mode =  GPIO_Mode_AF_PP;
    RX.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    RX.GPIO_Speed = TX.GPIO_Speed = GPIO_Speed_50MHz;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    if(usartn == USART1)
    {
        // init USART1 RX(PA10) and TX(PA9)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
        RX.GPIO_Pin = GPIO_Pin_10;
        GPIO_Init(GPIOA, &RX);
        TX.GPIO_Pin = GPIO_Pin_9;
        GPIO_Init(GPIOA, &TX);

        nvic.NVIC_IRQChannel = USART1_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
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

        nvic.NVIC_IRQChannel = USART2_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;
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

        nvic.NVIC_IRQChannel = USART3_IRQn;
        nvic.NVIC_IRQChannelCmd = ENABLE;

        // ========================================================================================================================================================
        // TODO: Add WLAN module initialisation
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

    usart.USART_BaudRate = baud;
	usart.USART_WordLength = USART_WordLength_8b;	 
	usart.USART_StopBits = USART_StopBits_1;
	usart.USART_Parity = USART_Parity_No;
	usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_Init(usartn, &usart);
    NVIC_Init(&nvic);

    /*if(usartn == USART1)      USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    else if(usartn == USART2) USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    else if(usartn == USART3) USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);*/
    USART_ITConfig(usartn, USART_IT_RXNE, ENABLE);

	USART_Cmd(usartn, ENABLE);
}

// ==================================================================================================================================================
// TODO: rewrite these 2 functions to handle WLAN module protocol
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

void SvVis3_t::init(USART_TypeDef *port, uint32_t baud)
{
    init_usart(port, baud);
    this->port = port;
    if(port == USART1)      _usart1_handler = this;
    else if(port == USART2) _usart2_handler = this;
    else if(port == USART3) _usart3_handler = this;

    this->queue_send  = osMessageQueueNew(SvVIS3_SEND_BACKLOG, sizeof(SvVis3_message_t), nullptr);
    this->queue_recv  = osMessageQueueNew(SvVIS3_RECV_BACKLOG, sizeof(SvVis3_message_t), nullptr);
    this->queue_usart = osMessageQueueNew(SvVis3_USART_BACKLOG, sizeof(char), nullptr);

    //this->thread_usart = osThreadNew(usart_thread, this, nullptr);
    this->thread_recv = osThreadNew(recv_thread, this, nullptr);
    this->thread_send = osThreadNew(send_thread, this, nullptr);
}

void SvVis3_t::send_msg(SvVis3_message_t &message)
{
    while(osMessageQueueGetSpace(this->queue_send) < 2) {osThreadYield();} // wait for available space in send queue
    osMessageQueuePut(this->queue_send, &message, nullptr, osWaitForever);
}
void SvVis3_t::send_str(const char *str)
{
    size_t len = strlen(str)+1;
	bool too_long = (len >= SvVIS3_DATA_MAX_LEN);
    if(too_long) {len = SvVIS3_DATA_MAX_LEN;}
    SvVis3_message_t message;
    message.channel = SvVIS3_STRING_CHANNEL;
    message.len = len;
    memcpy(message.data.raw, str, len);
	if(too_long) {message.data.raw[SvVIS3_DATA_MAX_LEN-1] = 0; /*USART_send_byte(USART1, '#');*/} else {/*USART_send_byte(USART1, '!');*/}
    this->send_msg(message);
}
void SvVis3_t::send_i16(SvVis3_channel_t channel, int16_t data)
{
    size_t len = sizeof(data);
    SvVis3_message_t message;
    message.channel = SvVIS3_I16_CHANNEL_BASE + channel;
    message.len = len;
    message.data.i16 = data;
    this->send_msg(message);
}
void SvVis3_t::send_i32(SvVis3_channel_t channel, int32_t data)
{
    size_t len = sizeof(data);
    SvVis3_message_t message;
    message.channel = SvVIS3_I32_CHANNEL_BASE + channel;
    message.len = len;
    message.data.i32 = data;
    this->send_msg(message);
}
void SvVis3_t::send_float(SvVis3_channel_t channel, float data)
{
    size_t len = sizeof(data);
    SvVis3_message_t message;
    message.channel = SvVIS3_FLOAT_CHANNEL_BASE + channel;
    message.len = len;
    message.data.f = data;
    this->send_msg(message);
}

void SvVis3_t::recv_msg(SvVis3_message_t &msg_buf)
{
    osMessageQueueGet(this->queue_recv, &msg_buf, NULL, osWaitForever);
}

void SvVis3_data_t::apply_msg(SvVis3_message_t &msg)
{
    if( (msg.channel >= SvVIS3_I16_CHANNEL_BASE) && (msg.channel < SvVIS3_I16_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 11..19
    {
        this->ch[msg.channel - SvVIS3_I16_CHANNEL_BASE].i16 = msg.data.i16;
        this->ch[msg.channel - SvVIS3_I16_CHANNEL_BASE].i32 = msg.data.i16;
        this->ch[msg.channel - SvVIS3_I16_CHANNEL_BASE].f   = msg.data.i16;
    }
    else if( (msg.channel >= SvVIS3_I32_CHANNEL_BASE) && (msg.channel < SvVIS3_I32_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 1..9
    {
        this->ch[msg.channel - SvVIS3_I32_CHANNEL_BASE].i16 = msg.data.i32;
        this->ch[msg.channel - SvVIS3_I32_CHANNEL_BASE].i32 = msg.data.i32;
        this->ch[msg.channel - SvVIS3_I32_CHANNEL_BASE].f   = msg.data.i32;
    }
    else if( (msg.channel >= SvVIS3_FLOAT_CHANNEL_BASE) && (msg.channel < SvVIS3_FLOAT_CHANNEL_BASE + SvVis3_CHANNEL_COUNT) ) // 21..29
    {
        this->ch[msg.channel - SvVIS3_FLOAT_CHANNEL_BASE].i16 = msg.data.f;
        this->ch[msg.channel - SvVIS3_FLOAT_CHANNEL_BASE].i32 = msg.data.f;
        this->ch[msg.channel - SvVIS3_FLOAT_CHANNEL_BASE].f   = msg.data.f;
    }
}
