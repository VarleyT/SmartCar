#include "whistle.h"

void WHISTLE_Config()
{
    RCC_APB2PeriphClockCmd(WHISTLE_CLK, ENABLE);
    GPIO_InitTypeDef typeDef;
    typeDef.GPIO_Mode = GPIO_Mode_Out_PP;
    typeDef.GPIO_Pin = WHISTLE_GPIO_Pin;
    typeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(WHISTLE_PORT, &typeDef);
}

void WHISTLE_Tip()
{
    GPIO_SetBits(WHISTLE_PORT, WHISTLE_GPIO_Pin);
    delay_ms(200);
    GPIO_ResetBits(WHISTLE_PORT, WHISTLE_GPIO_Pin);
    delay_ms(100);
    GPIO_SetBits(WHISTLE_PORT, WHISTLE_GPIO_Pin);
    delay_ms(200);
    GPIO_ResetBits(WHISTLE_PORT, WHISTLE_GPIO_Pin);
}