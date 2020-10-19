#include "SvVis_cortex.hpp"
#include "motor_driver.hpp"

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
        tar.init(USART1, USART_BAUD_9600);
        motor_init();
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
    tar->send("Supported Commands:");
    tar->send("STOP Stop");
    tar->send("FW   Move Forard");
    tar->send("BW   Move Backwards");
    tar->send("RR   Rotate Right");
    tar->send("RL   Rotate Left");
    for(;;)
    {
        tar->recv(msg);
        tar->send("confirm message");
        tar->send(msg);

        motor_cmd_str(msg.data.raw);
    }
}

