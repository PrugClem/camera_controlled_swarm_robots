#include "SvVis_cortex.hpp"
#include "motor_driver.h"
#include "LED_driver.h"

// main thread
__NO_RETURN void main_thread_func(void *arg);
osThreadId_t main_thread_handle;

// main
int main(void)
{
    SvVis_t tar[3];
    SystemCoreClockUpdate();
    if(osKernelInitialize() == osOK)
    {
        tar[0].init(USART1, USART_BAUD_9600); // initialise DAP USART
        tar[1].init(USART2, USART_BAUD_9600); // initialise Bluetooth USART
        tar[2].init(USART3, USART_BAUD_9600); // initialise WLAN USART

        main_thread_handle = osThreadNew(main_thread_func, tar, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
}

// main thread function
void main_thread_func(void *arg)
{
    SvVis_t *tar = (SvVis_t*)arg;
    SvVis_message_t msg;
    SvVis_t *sender = nullptr;

    motor_init();   // initialise motor driver
    LED_init();     // initialise LED driver and start heartbeat thread
    // if the system needs more memory, the heartbeat LED will not turn on

    // triangle LED test, only enable one led for 500ms then all at once
    LED_triangle_l(true, false, false);osDelay(500);
    LED_triangle_l(false, true, false);osDelay(500);
    LED_triangle_l(false, false, true);osDelay(500);
    LED_triangle_l(true, true, true);
#if 1
    for(;;)
    {
        // pointer sender point to an instance that has received a message
        // if the pointer is a nullptr, no instance has a message to receive
        // because of else if instead of regular if's the first instance gets priority
        sender = nullptr;
        if (tar[0].available() > 0)
        {
            sender = &tar[0];
        }
        else if (tar[1].available() > 0)
        {
            sender = &tar[1];
        }
        else if (tar[2].available() > 0)
        {
            sender = &tar[2];
        }

        if(sender == nullptr)
        {
            // if no sender was found, do nothing (yield to save CPU time)
            osThreadYield();
        }
        else
        {
            sender->recv_msg(msg);

            // debug output
            sender->send_str("confirm message");
            sender->send_msg(msg);

            if( strcmp(msg.data.raw, "help") == 0 )
            {
                // short help page
                sender->send_str("Supported Commands:");
                sender->send_str("stop   Stop");
                sender->send_str("fw <t> Move Forard");
                sender->send_str("bw <t> Move Backwards");
                sender->send_str("rr <t> Rotate Right");
                sender->send_str("rl <t> Rotate Left");
                sender->send_str("");
                sender->send_str("<t> indicates that it is");
                sender->send_str(" possible to run a command for");
                sender->send_str(" a limited time only");
                sender->send_str("");
            }
            else if(!motor_cmd_str(msg.data.raw))
            {
                // error output
                sender->send_str("unknown command");
            }
        }
    }
#else
    // old code which only uses one interface
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
#endif
}

