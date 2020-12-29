#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "cmsis_os2.h"
#include "misc.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

typedef uint8_t motor_cmd_bin_t;

void motor_init(void);
bool motor_cmd_str(const char* cmd);
bool motor_cmd_bin(motor_cmd_bin_t cmd, uint32_t time);

#define MOTOR_CMD_STOP ((motor_cmd_bin_t) 0)
#define MOTOR_CMD_FW   ((motor_cmd_bin_t) 1)
#define MOTOR_CMD_BW   ((motor_cmd_bin_t) 2)
#define MOTOR_CMD_RR   ((motor_cmd_bin_t) 3)
#define MOTOR_CMD_RL   ((motor_cmd_bin_t) 4)

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MOTOR_DRIVER_H
