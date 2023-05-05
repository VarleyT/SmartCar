#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define MOTOR_CLK                           RCC_APB2Periph_GPIOB
#define MOTOR_PORT                          GPIOB
#define MOTOR_GPIO_Pin_Left1				GPIO_Pin_5
#define MOTOR_GPIO_Pin_Left2				GPIO_Pin_6
#define MOTOR_GPIO_Pin_Right1				GPIO_Pin_7
#define MOTOR_GPIO_Pin_Right2				GPIO_Pin_8
#define MOTOR_GPIO_Pin_PWML					GPIO_Pin_14
#define MOTOR_GPIO_Pin_PWMR					GPIO_Pin_15

#define MOTOR_TIMx                          TIM3
#define MOTOR_TIM_APBxClockCmd              RCC_APB1PeriphClockCmd
#define MOTOR_TIM_CLK                       RCC_APB1Periph_TIM3
#define MOTOR_TIM_IRQn                      TIM3_IRQn
#define MOTOR_TIMx_IRQHandler               TIM3_IRQHandler

typedef enum
{
    MOTOR_DIRECTION_STOP = 0,
    MOTOR_DIRECTION_FORWARD = 1,
    MOTOR_DIRECTION_BACKWARD = 2,
    MOTOR_DIRECTION_LEFT = 3,
    MOTOR_DIRECTION_RIGHT = 4
} MOTOR_DIRECTION;

extern MOTOR_DIRECTION current_direction;

void MOTOR_Config(void);

void MOTOR_SetDirection(MOTOR_DIRECTION direction);

#endif