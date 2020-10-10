#include "motor_driver.h"
#include "SvVis_cortex.h"

void motor_init(void)
{
    GPIO_InitTypeDef gpio = {.GPIO_Mode = GPIO_Mode_Out_PP, .GPIO_Speed = GPIO_Speed_50MHz};
    //enable GPIOB and GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);

    //set PB6 and PB7 as output
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &gpio);

    // set PC0 - PC3 as output
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOC, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOC, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOC, &gpio);
}

void motor_cmd_str(const char* cmd)
{
    if( strcmp(cmd, "STOP") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_STOP);
    }
    else if( strcmp(cmd, "FW") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_FW);
    }
    else if( strcmp(cmd, "BW") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_BW);
    }
    else if( strcmp(cmd, "RR") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_RR);
    }
    else if( strcmp(cmd, "RL") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_RL);
    }
    else // unrecognized command, stopping
    {
        motor_cmd_bin(MOTOR_CMD_STOP);
    }
}

void motor_cmd_bin(motor_cmd_bin_t cmd)
{
    switch (cmd)
    {
    case MOTOR_CMD_STOP:
        GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
        break;
    case MOTOR_CMD_FW:
        GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_2);
        GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_3);
        break;
    case MOTOR_CMD_BW:
        GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_SetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_3);
        GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_2);
        break;
    case MOTOR_CMD_RR:
        GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_SetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2);
        GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_3);
        break;
    case MOTOR_CMD_RL:
        GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_3);
        GPIO_ResetBits(GPIOC, GPIO_Pin_1 | GPIO_Pin_2);

    default: // unrecognized command, stopping
        GPIO_ResetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);
        break;
    }
}
