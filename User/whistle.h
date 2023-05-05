#ifndef __WHISTLE_H
#define __WHISTLE_H

#include "sys.h"
#include "delay.h"

#define WHISTLE_CLK                     RCC_APB2Periph_GPIOB
#define WHISTLE_PORT                    GPIOB
#define WHISTLE_GPIO_Pin                GPIO_Pin_0

void WHISTLE_Config(void);

void WHISTLE_Tip(void);

#endif