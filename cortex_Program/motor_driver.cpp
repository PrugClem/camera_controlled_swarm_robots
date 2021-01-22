/**
 *  This file is responsible of initialising verything the motors need to run properly
 *  This file is also responsible for handling string/binary commands for the motor
 */

#include "motor_driver.hpp"
#include "extra.hpp"

double speed_mult = 1.0;
double base_speed_left = 0.1, base_speed_right = 0.1;
uint16_t timer_period = 4096;
osTimerId_t os_timer_stop;

void TIM3_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void motor_stop(void*arg)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
}

void timer_init(void)
{
    /**
     *  TIM3 CH3 (PB0): output PWM for motor enable for LEFT  motor
     *  TIM3 CH4 (PB1): output PWM for motor enable for RIGHT motor
     */
    TIM_TimeBaseInitTypeDef timer;
    TIM_OCInitTypeDef outputcompare;
    GPIO_InitTypeDef gpio;

    // set PB0, PB1 as output for motor enable signals
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // enable clock for gpiob
    gpio.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate function output (Output compare output)
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Pin = GPIO_Pin_0; // PB0
    GPIO_Init(GPIOB, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1; // PB1
    GPIO_Init(GPIOB, &gpio);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enable clock for timer
    timer.TIM_RepetitionCounter = 0; // infinite repetition
    timer.TIM_CounterMode = TIM_CounterMode_Up; // count upwards
    timer.TIM_ClockDivision = TIM_CKD_DIV1; // no clock division
    timer.TIM_Prescaler = 256; // prescaler
    timer.TIM_Period = timer_period; // autoreload
    TIM_TimeBaseInit(TIM3, &timer);

    memset(&outputcompare, 0, sizeof(outputcompare));
    outputcompare.TIM_OCMode = TIM_OCMode_PWM1;
    outputcompare.TIM_OutputState = TIM_OutputState_Enable;
    outputcompare.TIM_Pulse = timer.TIM_Period * base_speed_left * speed_mult;
    TIM_OC3Init(TIM3, &outputcompare);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

float speed_mult = 1.0;
    outputcompare.TIM_Pulse = timer.TIM_Period * base_speed_right * speed_mult;
    TIM_OC4Init(TIM3, &outputcompare);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

    TIM_Cmd(TIM3, ENABLE);
}

void motor_init(void)
{
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    //enable GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // set PA0,PA1, PA6,PA7 as output for motor control signals
    gpio.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOA, &gpio);
    gpio.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &gpio);

    //set PA4 as input floating because high impedance because hardware
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    gpio.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOA, &gpio);

    timer_init();

    os_timer_stop = osTimerNew(motor_stop, osTimerOnce, NULL, NULL);
}

bool motor_cmd_str(const char* cmd, SvVis_t *src)
{
    uint32_t time = osWaitForever;
    // after the end of the string, the buffer is consistently filled with zeros thanks to memset() in recv_thread() in SvVis_cortex_threads.cpp
    if( strncmp(cmd, "stop", 4) == 0 )
    {
        time = strtoul(cmd+5, NULL, 0); // "stop %d"
        motor_cmd_bin(MOTOR_CMD_STOP, time);
    }
    else if( strncmp(cmd, "fw", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "fw %d"
        motor_cmd_bin(MOTOR_CMD_FW, time);
    }
    else if( strncmp(cmd, "bw", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "bw %d"
        motor_cmd_bin(MOTOR_CMD_BW, time);
    }
    else if( strncmp(cmd, "rr", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "rr %d"
        motor_cmd_bin(MOTOR_CMD_RR, time);
    }
    else if( strncmp(cmd, "rl", 2) == 0 )
    {
        time = strtoul(cmd+3, NULL, 0); // "rl %d"
        motor_cmd_bin(MOTOR_CMD_RL, time);
    }
    else if( strncmp(cmd, "speed", 5) == 0 )
    {
        if(strlen(cmd) == 5)
        {
            char msg[32];
            //snprintf(msg, sizeof(msg), "current speed: %d", (int)(speed_mult * 128) );
            strcpy(msg, "current speed: ");
            ul_to_string(msg+15, sizeof(msg)-15, speed_mult*128);
            src->send_str(msg);
        }
        else
        {
            time = strtoul(cmd+6, NULL, 0); // "speed %d"
            motor_set_speed(time / 128.0);
        }
    }
    else // unrecognized command, stopping
    {
        motor_cmd_bin(MOTOR_CMD_STOP, osWaitForever);
        return false; // return false if an unknown command was used
    }
    return true; // return true if a valid command was received
}

bool motor_cmd_bin(motor_cmd_bin_t cmd, uint32_t time)
{
    switch (cmd)
    {
    case MOTOR_CMD_STOP:
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        motor_stop(NULL);
        break;
    case MOTOR_CMD_FW:
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_BW:
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        break;
    case MOTOR_CMD_RR:
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_RL:
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        break;

    default: // unrecognized command, stopping
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        motor_stop(NULL);
        return false; // return false if an unrecognized command was used
    }
    osTimerStart(os_timer_stop, time);
    return true; // return true if a valid command was used
}

void motor_update_speed(void)
{
    TIM_OCInitTypeDef outputcompare;

    memset(&outputcompare, 0, sizeof(outputcompare));
    outputcompare.TIM_OCMode = TIM_OCMode_PWM1;
    outputcompare.TIM_OutputState = TIM_OutputState_Enable;
    outputcompare.TIM_Pulse = timer_period * base_speed_left * speed_mult;
    TIM_OC3Init(TIM3, &outputcompare);

    outputcompare.TIM_Pulse = timer_period * base_speed_right * speed_mult;
    TIM_OC4Init(TIM3, &outputcompare);
}

void motor_set_speed(float new_speed)
{
    speed_mult = new_speed;
    motor_update_speed();
}
