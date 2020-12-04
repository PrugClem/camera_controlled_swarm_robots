#include "LED_driver.h"

void LED_init(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    // enable GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    //set PC0 as output for triangle LED #0
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOC, &gpio);
    //set PC1 as output for triangle LED #1
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &gpio);
    //set PC2 as output for triangle LED #2
    gpio.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &gpio);
    //set PC0 as output for Heartbeat LED
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &gpio);
}

void LED_heartbeat(bool newstate)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_3, (newstate) ? Bit_SET : Bit_RESET);
}

void LED_triangle_l(bool led0, bool led1, bool led2)
{
    GPIO_WriteBit(GPIOC, GPIO_Pin_0, (led0) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_1, (led1) ? Bit_SET : Bit_RESET);
    GPIO_WriteBit(GPIOC, GPIO_Pin_2, (led2) ? Bit_SET : Bit_RESET);
}
