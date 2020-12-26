#include "SvVis_cortex.hpp"
#include "motor_driver.h"
#include "LED_driver.h"

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
        tar.init(USART1, USART_BAUD_9600); // initialise communication

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

    LED_init();
    motor_init();

    // triangle LED test, only enable one led for 500ms then all at once
    LED_triangle_l(true, false, false);osDelay(500);
    LED_triangle_l(false, true, false);osDelay(500);
    LED_triangle_l(false, false, true);osDelay(500);
    LED_triangle_l(true, true, true);

    for(;;)
    {
        tar->recv_msg(msg);
        
        // debug output
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
        else if(!motor_cmd_str(msg.data.raw))
        {
            tar->send_str("unknown command");
        }
    }
}

