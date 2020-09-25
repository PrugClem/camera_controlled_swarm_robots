#include "SvVis_cortex.h"

// main thread
__NO_RETURN void main_thread_func(void *arg);
osThreadId_t main_thread_handle;

// LED blink thread
            void dil_led_init(void);
__NO_RETURN void dil_led_blink(void *arg);
osThreadId_t led_blink_handle;

// main
int main(void)
{
#if 0
    init_usart(USART1);
    USART_SendData(USART1, 'A');
    USART_send_byte(USART1, 'B');
#else
    SvVis3_t tar;
    SystemCoreClockUpdate();
    if(osKernelInitialize() == osOK)
    {
        SvVis3_init(&tar, USART1);
        main_thread_handle = osThreadNew(main_thread_func, &tar, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
#endif
}

// main thread function
void main_thread_func(void *arg)
{
    SvVis3_t *tar = arg;
    SvVis3_message_t msg;
    led_blink_handle = osThreadNew(dil_led_blink, NULL, NULL);
    SvVis3_send_string(tar, "Hello", osWaitForever);
    SvVis3_send_string(tar, "abcdefghijklmnopqrstuvwxyz0123456789", osWaitForever);
    for(;;)
    {
        SvVis3_recv_message(tar, &msg, osWaitForever);
        SvVis3_send_string(tar, "message received:", osWaitForever);
        SvVis3_send_I16(tar, 0, strlen(msg.data.raw)+1, osWaitForever);
        SvVis3_send_I32(tar, 2, strlen(msg.data.raw)+1, osWaitForever);
        SvVis3_send_float(tar, 1, strlen(msg.data.raw)+1, osWaitForever);
        SvVis3_send_message(tar, &msg, osWaitForever);
        //osDelay(100);
    }
}

void dil_led_init(void)
{
    // PD2
    GPIO_InitTypeDef gpio;

    // enable cllock for GPIOD
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    // Set Port D2 as General purpose push pull
    gpio.GPIO_Pin = GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &gpio);
}
void dil_led_blink(void *arg)
{
    dil_led_init();
    for(;;)
    {
        GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_SET);
        osDelay(200);
        GPIO_WriteBit(GPIOD, GPIO_Pin_2, Bit_RESET);
        osDelay(200);
    }
}
