/**
 *  This file ist the entry point for the Program
 *  This file is responsible for Initialising and starting the CMSIS RTOS kernel
 *  This file is also responsible for handling the communication to the different interfaces
 */

#include "SvVis_cortex.hpp"
#include "motor_driver.h"
#include "LED_driver.h"

// main thread
__NO_RETURN void main_thread_func(void *arg);
osThreadId_t main_thread_handle;

struct SvVis_collection // used for easyer use in programming
{
    SvVis_t daplink;    // USART1 via DAPlink
    SvVis_t bluetooth;  // USART2 via HC06 bluetooth module
    SvVis_t wlan;       // USART3 via ESP-8266 WLAN module
};


// main
int main(void)
{
    SvVis_collection tar;
    SystemCoreClockUpdate();
    if(osKernelInitialize() == osOK)
    {
        LED_init();     // initialise LED driver and start heartbeat thread
        tar.daplink.init(USART1, USART_BAUD_9600); // initialise DAP USART
        tar.bluetooth.init(USART2, USART_BAUD_9600); // initialise Bluetooth USART
        tar.wlan.init(USART3, USART_BAUD_115200); // initialise WLAN USART

        main_thread_handle = osThreadNew(main_thread_func, &tar, NULL);
        osKernelStart();
    }
    for(;;); // This code is only reached in an error
}

// main thread function
void main_thread_func(void *arg)
{
    SvVis_collection *tar = (SvVis_collection*)arg; // retrieve the communication handles
    SvVis_message_t msg; // buffer for processing incoming messages
    SvVis_t *sender = nullptr; // pointer to send data back

    motor_init();   // initialise motor driver

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
        if (tar->daplink.available() > 0)
        {
            sender = &tar->daplink;
        }
        else if (tar->bluetooth.available() > 0)
        {
            sender = &tar->bluetooth;
        }
        else if (tar->wlan.available() > 0)
        {
            sender = &tar->wlan;
        }

        if(sender == nullptr)
        {
            // if no sender was found, do nothing (yield to save CPU time)
            // this is reached if no interface has received data
            // if 2 interfaced have received data, no yield is called between handling the messages
            osThreadYield();
        }
        else
        {
            // some interface has received data, due to the classes the retrieving is the same for all interfaces
            sender->recv_msg(msg); // receive a message

            // debug output
            sender->send_str("confirm message"); // confim the received message
            sender->send_msg(msg);               // can be removed, this is mainly for testing puropses

            if( msg.is_string() ) // only process string messages
            {
                if( strcmp(msg.data.raw, "help") == 0 )
                {
                    // "short" help page
                    sender->send_str("Supported Commands:");
                    sender->send_str("stop   Stop");
                    sender->send_str("fw <t> Move Forard");
                    sender->send_str("bw <t> Move Backwards");
                    sender->send_str("rr <t> Rotate Right");
                    sender->send_str("rl <t> Rotate Left");
                    sender->send_str(" ");
                    sender->send_str("<t> indicates that it is");
                    sender->send_str(" possible to run a command for");
                    sender->send_str(" a limited time only");
                    sender->send_str(" ");
                }
                else if(!motor_cmd_str(msg.data.raw)) // run a motor command
                {
                    // error output
                    sender->send_str("unknown command");
                }
            } // only process string messages
        } // some interface has receved data
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

