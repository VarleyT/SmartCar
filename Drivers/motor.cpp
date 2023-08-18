//
// Created by 19634 on 2023/8/10.
//

#include "motor.h"

MOTOR::MOTOR() {
    this->current_direction = MOTOR_DIRECTION_FORWARD;
    this->current_pwm_l = 50;
    this->current_pwm_r = 50;
    MOTOR_APBxClockCmd(MOTOR_CLK, ENABLE);
    MOTOR_TIM_APBxClockCmd(MOTOR_TIM_CLK, ENABLE);
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Left2 |
                                  MOTOR_GPIO_Pin_Right1 | MOTOR_GPIO_Pin_Right2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = MOTOR_GPIO_Pin_PWML | MOTOR_GPIO_Pin_PWMR;
    GPIO_Init(MOTOR_PORT, &GPIO_InitStructure);
    //定时器初始化
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    TIM_InitStructure.TIM_Prescaler = MOTOR_TIM_PRESCALER;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_Period = MOTOR_TIM_PERIOD;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_InitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(MOTOR_TIMx, &TIM_InitStructure);

    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = MOTOR_TIM_PULSE;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC3Init(MOTOR_TIMx, &TIM_OCInitStructure);
    TIM_OC4Init(MOTOR_TIMx, &TIM_OCInitStructure);

    TIM_OC3PreloadConfig(MOTOR_TIMx, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(MOTOR_TIMx, TIM_OCPreload_Enable);
    TIM_Cmd(MOTOR_TIMx, ENABLE);

    TIM_SetCompare3(MOTOR_TIMx, this->current_pwm_l);
    TIM_SetCompare4(MOTOR_TIMx, this->current_pwm_r);
}

void MOTOR::setDirection(MOTOR_DIRECTION direction) {
    switch (direction) {
        case MOTOR_DIRECTION_STOP:
            current_direction = MOTOR_DIRECTION_STOP;
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Left2 |
                                       MOTOR_GPIO_Pin_Right1 | MOTOR_GPIO_Pin_Right2);
            break;
        case MOTOR_DIRECTION_FORWARD:
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Right1);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2 | MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_FORWARD;
            break;
        case MOTOR_DIRECTION_BACKWARD:
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Right1);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2 | MOTOR_GPIO_Pin_Right2);
            current_direction = MOTOR_DIRECTION_BACKWARD;
            break;
        case MOTOR_DIRECTION_LEFT:
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Right2);
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2 | MOTOR_GPIO_Pin_Right1);
            current_direction = MOTOR_DIRECTION_LEFT;
            break;
        case MOTOR_DIRECTION_RIGHT:
            GPIO_SetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left1 | MOTOR_GPIO_Pin_Right2);
            GPIO_ResetBits(MOTOR_PORT, MOTOR_GPIO_Pin_Left2 | MOTOR_GPIO_Pin_Right1);
            current_direction = MOTOR_DIRECTION_RIGHT;
            break;
    }
}

MOTOR_DIRECTION MOTOR::getDirection() const {
    return current_direction;
}

void MOTOR::setSpeed(u16 pwm_l,u16 pwm_r) {
    this->current_pwm_l = pwm_l;
    this->current_pwm_r = pwm_r;
    TIM_SetCompare3(MOTOR_TIMx, this->current_pwm_l);
    TIM_SetCompare4(MOTOR_TIMx, this->current_pwm_r);
}


u16 MOTOR::getSpeed_l() const {
    return current_pwm_l;
}

u16 MOTOR::getSpeed_r() const {
    return current_pwm_r;
}
