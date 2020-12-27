#ifndef USART_DRIVER_H_INCLUDED
#define USART_DRIVER_H_INCLUDED

#include "misc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

void init_usart(USART_TypeDef *usartn, uint32_t baud);
void USART_send_bytes(USART_TypeDef* port, void *start, size_t len);
void USART_send_byte(USART_TypeDef* port, uint8_t byte);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USART_DRIVER_H_INCLUDED
