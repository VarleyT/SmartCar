#include "motor.h"

MOTOR_DIRECTION current_direction;
u32 MOTOR_PWM = 350;

void MOTOR_GPIO_Init(u16 MOTOR_GPIO_Pin)
{
    GPIO_InitTypeDef typeDef;
    typeDef.GPIO_Mode = GPIO_Mode_Out_PP;
    typeDef.GPIO_Pin = MOTOR_GPIO_Pin;
    typeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT, &typeDef);
}

void MOTOR_GPIO_Clear()
{
    GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1);
    GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2);
    GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right1);
    GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right2);
}

void MOTOR_Config()
{
    MOTOR_APBxClockCmd(MOTOR_CLK, ENABLE);
    MOTOR_TIM_APBxClockCmd(MOTOR_TIM_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Left2 | MOTOR_GPIO_Pin_Right1 |
        MOTOR_GPIO_Pin_Right2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_Pin_PWML | MOTOR_GPIO_Pin_PWMR;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Period = MOTOR_TIM_PERIOD;
    TIM_InitStructure.TIM_Prescaler = MOTOR_TIM_PRESCALER;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_TIMx, &TIM_InitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       //输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出通道电平极性
    TIM_OCInitStructure.TIM_Pulse = MOTOR_TIM_PULSE;        //空占比
    TIM_OC3Init(MOTOR_TIMx, &TIM_OCInitStructure);
    TIM_OC4Init(MOTOR_TIMx, &TIM_OCInitStructure);
 
    TIM_OC3PreloadConfig(MOTOR_TIMx, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(MOTOR_TIMx, TIM_OCPreload_Enable);
    TIM_Cmd(MOTOR_TIMx, ENABLE);
}

void MOTOR_SetDirection(MOTOR_DIRECTION direction)
{
    TIM_Cmd(MOTOR_TIMx, DISABLE);
    MOTOR_GPIO_Clear();
    switch(direction)
    {
        case MOTOR_DIRECTION_STOP:
            current_direction = MOTOR_DIRECTION_STOP;
            break;
        case MOTOR_DIRECTION_FORWARD:
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right1);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_FORWARD;
            break;
        case MOTOR_DIRECTION_BACKWARD:
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right1);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_BACKWARD;
            break;
        case MOTOR_DIRECTION_LEFT:
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right1);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_LEFT;
            break;
        case MOTOR_DIRECTION_RIGHT:
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right1);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_RIGHT;
            break;
    }
    TIM_Cmd(MOTOR_TIMx, ENABLE);
}

void MOTOR_SetSpeed(u32 pwm)
{
    MOTOR_PWM = pwm;
    TIM_SetCompare3(MOTOR_TIMx, MOTOR_PWM);
    TIM_SetCompare4(MOTOR_TIMx, MOTOR_PWM);
}