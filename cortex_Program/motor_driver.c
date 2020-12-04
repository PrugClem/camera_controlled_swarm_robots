#include "motor_driver.h"

const uint32_t CYCLE_PERIOD = 10;
double DUTY_CYCLE = 1.0;
bool running = false;

__NO_RETURN void generate_pwm(void *arg)
{
    while (true)
    {
        uint32_t on_time = CYCLE_PERIOD * DUTY_CYCLE;
        uint32_t off_time = CYCLE_PERIOD - on_time;
        if(running)
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
        osDelay(on_time);
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        osDelay(off_time);
    }
    
}

void motor_init(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    //enable GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    //set PA4 as output for enable signal
    gpio.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &gpio);

    // set PA0,PA1, PA6,PA7 as output for motor control signals
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);

    osThreadNew(generate_pwm, NULL, NULL);
}

void motor_cmd_str(const char* cmd)
{
    if( strcmp(cmd, "stop") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_STOP);
    }
    else if( strcmp(cmd, "fw") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_FW);
    }
    else if( strcmp(cmd, "bw") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_BW);
    }
    else if( strcmp(cmd, "rr") == 0 )
    {
        motor_cmd_bin(MOTOR_CMD_RR);
    }
    else if( strcmp(cmd, "rl") == 0 )
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
        // Disable Motor enable Signal
        running = false;
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_FW:
        // Enable Motor enable Signal
        running = true;
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_BW:
        // Enable Motor enable Signal
        running = true;
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        break;
    case MOTOR_CMD_RR:
        // Enable Motor enable Signal
        running = true;
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_RL:
        // Enable Motor enable Signal
        running = true;
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);

    default: // unrecognized command, stopping
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        break;
    }
}
