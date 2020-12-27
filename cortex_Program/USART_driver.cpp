#include "USART_driver.hpp"
#include "SvVis_cortex.hpp" // for using the SvVis queues

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
