#ifndef __USART_H
#define __USART_H

#include "sys.h"
#include "timer.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

#define USARTx                          USART1
#define USART_GPIO_PORT                 GPIOA

#define USART_GPIO_CLK                  (RCC_APB2Periph_GPIOA)
#define USART_GPIO_APBxClkCmd           RCC_APB2PeriphClockCmd
#define USART_CLK                       (RCC_APB2Periph_USART1)
#define USART_APBxClkCmd                RCC_APB2PeriphClockCmd

#define USART_GPIO_TX_Pin               GPIO_Pin_9
#define USART_GPIO_RX_Pin               GPIO_Pin_10

#define USART_BAUDRATE                  115200

#define USART_IRQ                       USART1_IRQn
#define USART_IRQHandler                USART1_IRQHandler

#define USART_MAX_RECV_LEN		        3000				//最大接收缓存字节数
#define USART_MAX_SEND_LEN		        600					//最大发送缓存字节数

extern u8 USART_RX_BUF[USART_MAX_RECV_LEN];         //接收缓冲,最大USART3_MAX_RECV_LEN字节
extern u8 USART_TX_BUF[USART_MAX_SEND_LEN];         //发送缓冲,最大USART3_MAX_SEND_LEN字节
extern vu16 USART_RX_STA;                           //接收数据状态
extern u16 direction;

void USART_Config(void);

void USART_Printf(char *fmt, ...);

#endif