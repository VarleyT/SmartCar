//
// Created by 19634 on 2023/8/17.
//

#include "dht11.h"

DHT11::DHT11(uint32_t RCC_APB2Periph_x, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
        : GPIOx(GPIOx), GPIO_Pin(GPIO_Pin) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_x, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void DHT11::setPinMode(GPIOMode_TypeDef mode) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = mode;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin;
    GPIO_Init(GPIOx, &GPIO_InitStructure);
}

u8 DHT11::startMeasure() {
    setPinMode(GPIO_Mode_Out_PP);   //输出模式
    GPIO_ResetBits(GPIOx, GPIO_Pin);    //输出20ms低电平后拉高
    Delay_ms(20);
    GPIO_SetBits(GPIOx, GPIO_Pin);

    setPinMode(GPIO_Mode_IN_FLOATING);  //输入模式
    Delay_us(20);
    if (!GPIO_ReadInputDataBit(GPIOx, GPIO_Pin)) {
        while (!GPIO_ReadInputDataBit(GPIOx, GPIO_Pin));
        while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin));
        return SUCCESS;
    }
    return ERROR;
}

u8 DHT11::receiveByteData() {
    u8 temp = 0;
    for (int i = 0; i < 8; i++) {
        temp <<= 1;
        while (!GPIO_ReadInputDataBit(GPIOx, GPIO_Pin));
        Delay_us(28);
        GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) ? (temp |= 0x01) : (temp &= ~0x01);
        while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin));
    }
    return temp;
}

DHT11Data_Struct DHT11::getData() {
    if (startMeasure() == SUCCESS) {
        u8 buf[5];
        buf[0] = receiveByteData();
        buf[1] = receiveByteData();
        buf[2] = receiveByteData();
        buf[3] = receiveByteData();
        buf[4] = receiveByteData();
        if (buf[0] + buf[1] + buf[2] + buf[3] == buf[4]) {
            return {static_cast<float>(buf[0] + (buf[1] % 100) * 0.01),
                    static_cast<float>(buf[2] + (buf[3] % 100) * 0.01)};
        }
    }
    return {};
}
