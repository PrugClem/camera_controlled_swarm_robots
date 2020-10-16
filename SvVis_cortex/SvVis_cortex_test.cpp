#include "SvVis_cortex.hpp"

// main thread
__NO_RETURN void main_thread_func(void *arg);
osThreadId_t main_thread_handle;

// main
int main(void)
{
    SvVis3_t tar;
    SystemCoreClockUpdate();
    if(osKernelInitialize() == osOK)
    {
        tar.init(USART1, USART_BAUD_115200);
        main_thread_handle = osThreadNew(main_thread_func, &tar, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
}

// main thread function
void main_thread_func(void *arg)
{
    SvVis3_t *tar = (SvVis3_t*)arg;
    SvVis3_message_t msg;
    tar->send("Hello");
    tar->send("abcdefghijklmnopqrstuvwxyz0123456789");
    for(;;)
    {
        tar->recv(msg);
        tar->send("message received:");
        tar->send(0, (int16_t)strlen(msg.data.raw)+1);
        tar->send(2, (int32_t)strlen(msg.data.raw)+1);
        tar->send(1, (float)strlen(msg.data.raw)+1);
        tar->send(msg);
        //osDelay(100);
    }
}