//
// Created by 19634 on 2023/8/17.
//

#ifndef __DHT11_H
#define __DHT11_H

#include "sys.h"
#include "delay.h"

typedef struct DHT11Data_Struct {
    float humidity;
    float temperature;
} DHT11Data_Struct;

class DHT11 {
private:
    GPIO_TypeDef *GPIOx;
    uint16_t GPIO_Pin;

    void setPinMode(GPIOMode_TypeDef mode);

    u8 startMeasure();

    u8 receiveByteData();

public:
    DHT11(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

    DHT11Data_Struct getData();
};


#endif //__DHT11_H
