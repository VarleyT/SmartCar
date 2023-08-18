//
// Created by 19634 on 2023/8/10.
//

#ifndef __LED_H
#define __LED_H

#include "sys.h"

#define LED_ON      HIGH
#define LED_OFF     LOW

class LED {
private:
    uint32_t RCC_APB2Periph_x;
    GPIO_TypeDef *GPIOx;
    u16 GPIO_Pin;

public:
    LED(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    void on();

    void off();

    void toggle();
};


#endif //__LED_H
