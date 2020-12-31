/**
 *  This file is responsible for interfacing the ESP-8266
 *  This file takes the raw data from the queue from the USART driver, and puts the decoded data into a seperate message queue
 * 
 *  The SvVis class is responsible for initialising the USART driver AND WLAN driver
 */

#include "USART_driver.hpp"
#include "SvVis_cortex.hpp"

#include <stdio.h>

void init_wlan()
{
    // USART connection via USART3 is ready to use
    queue_usart3 = osMessageQueueNew(SvVIS_WLAN_BACKLOG, sizeof(char), nullptr);
    return;
}

void _WLAN_run(const char *cmd)
{
    USART_send_bytes(USART3, cmd, strlen(cmd)); // the command itself
    USART_send_bytes(USART3, "\r\n", 2); // terminates the command sequence
    //while(osMessageQueueGetCount(queue_wlan_response) == 0) {osThreadYield();}
}

void WLAN_connect(const char* rem_ip, uint16_t rem_port)
{
    char cmdbuf[256];
    sprintf(cmdbuf, "AT+CIPSTART=\"TCP\",\"%s\",\"%d\"", rem_ip, rem_port);
    _WLAN_run(cmdbuf);
}

void WLAN_send_byte(uint8_t byte)
{
    return;
}
void WLAN_send_bytes(void *start, size_t len)
{
    return;
}
