/**
 *  This file is responsible of initialising verything the motors need to run properly
 *  This file is also responsible for handling string/binary commands for the motor
 */

#include "motor_driver.h"

double duty_cycle_left = 0.05, duty_cycle_right = 0.05;
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
    timer.TIM_Period = 4096; // autoreload
    TIM_TimeBaseInit(TIM3, &timer);

    outputcompare.TIM_OCMode = TIM_OCMode_PWM1;
    outputcompare.TIM_OCIdleState = TIM_OCIdleState_Reset;
    outputcompare.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    outputcompare.TIM_OCNPolarity = TIM_OCNPolarity_High;
    outputcompare.TIM_OCPolarity = TIM_OCPolarity_High;
    outputcompare.TIM_OutputNState = TIM_OutputNState_Disable;
    outputcompare.TIM_OutputState = TIM_OutputState_Enable;
    outputcompare.TIM_Pulse = timer.TIM_Period * duty_cycle_left;
    TIM_OC3Init(TIM3, &outputcompare);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

    outputcompare.TIM_Pulse = timer.TIM_Period * duty_cycle_right;
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

bool motor_cmd_str(const char* cmd)
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
        // Disable Motor enable Signal
        //GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6 | GPIO_Pin_7);
        motor_stop(NULL);
        break;
    case MOTOR_CMD_FW:
        // Enable Motor enable Signal
        GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_BW:
        // Enable Motor enable Signal
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_7);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_6);
        break;
    case MOTOR_CMD_RR:
        // Enable Motor enable Signal
        GPIO_SetBits(GPIOA, GPIO_Pin_1 | GPIO_Pin_6);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_7);
        break;
    case MOTOR_CMD_RL:
        // Enable Motor enable Signal
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
