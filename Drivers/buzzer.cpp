//
// Created by 19634 on 2023/8/12.
//

#include "buzzer.h"

BUZZER::BUZZER() = default;

BUZZER::BUZZER(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
        : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_x, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void BUZZER::on() {
#if BUZZER_ON
    GPIO_SetBits(GPIOx, GPIO_Pin);
#else
    GPIO_ResetBits(GPIOx, GPIO_Pin);
#endif
}

void BUZZER::off() {
#if BUZZER_OFF
    GPIO_SetBits(GPIOx, GPIO_Pin);
#else
    GPIO_ResetBits(GPIOx, GPIO_Pin);
#endif
}


void BUZZER::toggle() {
    if (GPIO_ReadOutputDataBit(GPIOx, GPIO_Pin)) {
        GPIO_ResetBits(GPIOx, GPIO_Pin);
    } else {
        GPIO_SetBits(GPIOx, GPIO_Pin);
    }
}


void BUZZER::playNormalSound() {
    repeat(2, [this] {
        on();
        Delay_ms(BUZZER_NORMAL_DURATION);
        off();
        Delay_ms(BUZZER_NORMAL_DURATION);
    });
}

void BUZZER::playErrorSound() {
    repeat(3, [this] {
        on();
        Delay_ms(BUZZER_ERROR_DURATION);
        off();
        Delay_ms(BUZZER_ERROR_DURATION);
    });
}