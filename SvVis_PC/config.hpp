#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

#include <cstdint>

#define SvVIS3_DATA_MAX_LEN 32
#define SvVIS3_SEND_BACKLOG 4
#define SvVIS3_RECV_BACKLOG 4
#define SvVis3_USART_BACKLOG 32

#define SvVis3_CHANNEL_COUNT 9
#define SvVIS3_I32_CHANNEL_BASE 1
#define SvVIS3_I16_CHANNEL_BASE 11
#define SvVIS3_STRING_CHANNEL 10
#define SvVIS3_FLOAT_CHANNEL_BASE 21

typedef uint8_t SvVis3_channel_t;

#endif // CONFIG_H_INCLUDED
