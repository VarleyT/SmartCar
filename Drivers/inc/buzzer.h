//
// Created by 19634 on 2023/8/12.
//

#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"
#include "delay.h"

#define BUZZER_ON                      HIGH
#define BUZZER_OFF                     LOW

#define BUZZER_NORMAL_DURATION         120
#define BUZZER_ERROR_DURATION          500

class BUZZER {
private:
    GPIO_TypeDef *GPIOx{};
    uint16_t GPIO_Pin{};

public:
    BUZZER();

    BUZZER(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    void on();

    void off();

    void toggle();

    void playNormalSound();

    void playErrorSound();
};

#endif //__BUZZER_H
