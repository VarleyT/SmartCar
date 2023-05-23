#ifndef __BUZZER_H
#define __BUZZER_H

#include "sys.h"
#include "delay.h"

#define BUZZER_CLK                     RCC_APB2Periph_GPIOB
#define BUZZER_PORT                    GPIOB
#define BUZZER_GPIO_Pin                GPIO_Pin_0

#define BUZZER_NORMAL_DURATION         120
#define BUZZER_ERROR_DURATION          500

void BUZZER_Config(void);

void BUZZER_PlayNormalSound(void);

void BUZZER_PlayErrorSound(void);

#endif