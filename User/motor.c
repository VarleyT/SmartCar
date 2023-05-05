#include "motor.h"

MOTOR_DIRECTION current_direction;

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
    RCC_APB2PeriphClockCmd(MOTOR_CLK, ENABLE);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_Left1);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_Left2);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_Right1);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_Right2);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_PWML);
    MOTOR_GPIO_Init(MOTOR_GPIO_Pin_PWMR);
    MOTOR_GPIO_Clear();
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