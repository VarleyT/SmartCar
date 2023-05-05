#ifndef __TIMER_H
#define __TIMER_H

#include "sys.h"

#define TIMERx                      TIM4
#define TIMER_APBxClockCmd          RCC_APB1PeriphClockCmd
#define TIMER_CLK                   RCC_APB1Periph_TIM4
#define TIMER_IRQn                  TIM4_IRQn
#define TIMERx_IRQHandler           TIM4_IRQHandler

extern vu16 USART_RX_STA;

void TIMER_Config(u16 period, u16 perscaler);

#endif