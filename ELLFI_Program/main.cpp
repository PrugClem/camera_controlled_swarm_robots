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
        tar.init(USART2, USART_BAUD_9600);
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
    for(;;)
    {
        tar->recv_msg(msg);
        
        tar->send_str("confirm message");
        tar->send_msg(msg);

        if(strcmp(msg.data.raw, "help") == 0)
        {
            tar->send_str("Supported Commands:");
            tar->send_str("stop Stop");
            tar->send_str("fw   Move Forard");
            tar->send_str("bw   Move Backwards");
            tar->send_str("rr   Rotate Right");
            tar->send_str("rl   Rotate Left");
        }

        motor_cmd_str(msg.data.raw);
    }
}

