#include "SvVis_cortex.hpp"
#include "motor_driver.h"
#include "LED_driver.h"

// main thread
__NO_RETURN void main_thread_func(void *arg);
__NO_RETURN void LED_heartbeat(void *arg);
osThreadId_t main_thread_handle;
osThreadId_t heartbeat_thread_handle;

// main
int main(void)
{
    SvVis3_t tar;
    SystemCoreClockUpdate();
    if(osKernelInitialize() == osOK)
    {
        tar.init(USART1, USART_BAUD_9600);
        motor_init();
        LED_init();

        LED_triangle_l(true, false, false);osDelay(1000);
        LED_triangle_l(false, true, false);osDelay(1000);
        LED_triangle_l(false, false, true);osDelay(1000);
        LED_triangle_l(true, true, true);

        main_thread_handle = osThreadNew(main_thread_func, &tar, NULL);
        heartbeat_thread_handle = osThreadNew(LED_heartbeat, NULL, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
}

void LED_heartbeat(void *arg)
{
    for(;;)
    {
        LED_heartbeat(true);
        osDelay(500);
        LED_heartbeat(false);
        osDelay(500);
    }
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

