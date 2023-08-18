//
// Created by 19634 on 2023/8/10.
//

#ifndef __USART1_H
#define __USART1_H

#include "sys.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define USART1_GPIO_PORT                 GPIOA
#define USART1_GPIO_CLK                  (RCC_APB2Periph_GPIOA)
#define USART1_GPIO_APBxClkCmd           RCC_APB2PeriphClockCmd
#define USART1_CLK                       (RCC_APB2Periph_USART1)
#define USART1_APBxClkCmd                RCC_APB2PeriphClockCmd

#define USART1_GPIO_TX_Pin               GPIO_Pin_9
#define USART1_GPIO_RX_Pin               GPIO_Pin_10

#define USART1_TIMERx                    TIM4
#define USART1_TIMER_APBxClockCmd        RCC_APB1PeriphClockCmd
#define USART1_TIMER_CLK                 RCC_APB1Periph_TIM4
#define USART1_TIMER_IRQn                TIM4_IRQn
#define USART1_TIMER_IRQHandler          TIM4_IRQHandler

#define USART1_BAUDRATE                  115200

#define USART1_MAX_RECV_LEN              3000
#define USART1_MAX_SEND_LEN              600


extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN];
extern vu16 USART1_RX_STA;

void USART1_Config();

void USART1_Printf(const char *fmt, ...);

u8 USART1_CheckData(const char *str);

void USART1_ClearRecvBuf();

#endif //__USART1_H
