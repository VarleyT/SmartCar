//
// Created by 19634 on 2023/8/10.
//

#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"

#define MOTOR_APBxClockCmd                  RCC_APB2PeriphClockCmd
#define MOTOR_CLK                           RCC_APB2Periph_GPIOB
#define MOTOR_PORT                          GPIOB
#define MOTOR_GPIO_Pin_Left1                GPIO_Pin_5
#define MOTOR_GPIO_Pin_Left2                GPIO_Pin_6
#define MOTOR_GPIO_Pin_Right1               GPIO_Pin_7
#define MOTOR_GPIO_Pin_Right2               GPIO_Pin_8
#define MOTOR_GPIO_Pin_PWML                 GPIO_Pin_0
#define MOTOR_GPIO_Pin_PWMR                 GPIO_Pin_1

#define MOTOR_TIMx                          TIM3
#define MOTOR_TIM_APBxClockCmd              RCC_APB1PeriphClockCmd
#define MOTOR_TIM_CLK                       RCC_APB1Periph_TIM3
#define MOTOR_TIM_PERIOD                    (101 - 1)
#define MOTOR_TIM_PRESCALER                 (3600 - 1) * 4
#define MOTOR_TIM_PULSE                     20
//#define MOTOR_TIM_IRQn                      TIM3_IRQn
//#define MOTOR_TIMx_IRQHandler               TIM3_IRQHandler

typedef enum {
    MOTOR_DIRECTION_STOP = 0,
    MOTOR_DIRECTION_FORWARD = 1,
    MOTOR_DIRECTION_BACKWARD = 2,
    MOTOR_DIRECTION_LEFT = 3,
    MOTOR_DIRECTION_RIGHT = 4
} MOTOR_DIRECTION;

class MOTOR {
private:
    u16 current_pwm_l;
    u16 current_pwm_r;
    MOTOR_DIRECTION current_direction;

public:
    MOTOR();

    void setDirection(MOTOR_DIRECTION direction);

    void setSpeed(u16 pwm_l,u16 pwm_r);

    MOTOR_DIRECTION getDirection() const;

    u16 getSpeed_l() const;

    u16 getSpeed_r() const;
};


#endif //__MOTOR_H
