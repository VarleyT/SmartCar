//
// Created by 19634 on 2023/8/10.
//

#include "led.h"

LED::LED(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
        : RCC_APB2Periph_x(RCC_APB2Periph_x), GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_x, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void LED::on() {
#if LED_ON
    GPIO_SetBits(GPIOx, GPIO_Pin);
#else
    GPIO_ResetBits(GPIOx, GPIO_Pin);
#endif
}

void LED::off() {
#if LED_OFF
    GPIO_SetBits(GPIOx, GPIO_Pin);
#else
    GPIO_ResetBits(GPIOx, GPIO_Pin);
#endif
}


void LED::toggle() {
    if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)) {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    } else {
        GPIO_SetBits(GPIOx, GPIO_Pin);
    }
}
