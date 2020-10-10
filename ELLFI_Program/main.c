#include "SvVis_cortex.h"
#include "motor_driver.h"

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
        SvVis3_init(&tar, USART1, USART_BAUD_9600);
        motor_init();
        main_thread_handle = osThreadNew(main_thread_func, &tar, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
}

// main thread function
void main_thread_func(void *arg)
{
    SvVis3_t *tar = arg;
    SvVis3_message_t msg;
    SvVis3_send_string(tar, "Supported Commands:");
    SvVis3_send_string(tar, "STOP Stop");
    SvVis3_send_string(tar, "FW   Move Forard");
    SvVis3_send_string(tar, "BW   Move Backwards");
    SvVis3_send_string(tar, "RR   Rotate Right");
    SvVis3_send_string(tar, "RL   Rotate Left");
    for(;;)
    {
        SvVis3_recv_message(tar, &msg, osWaitForever);
        SvVis3_send_string(tar, "confirm message");
        SvVis3_send_message(tar, &msg);

        motor_cmd_str(msg.data.raw);
    }
}

