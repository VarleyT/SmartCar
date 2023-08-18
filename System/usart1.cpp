//
// Created by 19634 on 2023/8/10.
//

#include "usart1.h"

u8 USART1_RX_BUF[USART1_MAX_RECV_LEN];
u8 USART1_TX_BUF[USART1_MAX_SEND_LEN];
vu16 USART1_RX_STA = 0;

void USART1_TIMER_Config(u16 period, u16 perscaler);

void USART1_Config() {
    USART1_GPIO_APBxClkCmd(USART1_GPIO_CLK, ENABLE);
    USART1_APBxClkCmd(USART1_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //GPIO_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = USART1_GPIO_TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);
    //GPIO_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = USART1_GPIO_RX_Pin;
    GPIO_Init(USART1_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USART1_BAUDRATE;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    USART1_TIMER_Config(1000 - 1, 7200 - 1);    //定时10ms
}

void USART1_Printf(const char *fmt, ...) {
    va_list list;
    va_start(list, fmt);
    vsprintf((char *) USART1_TX_BUF, fmt, list);
    va_end(list);

    u16 length, i;
    length = strlen((const char *) USART1_TX_BUF);
    USART1_ClearRecvBuf();
    for (i = 0; i < length; i++) {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        USART_SendData(USART1, USART1_TX_BUF[i]);
    }
}

u8 USART1_CheckData(const char *str) {
    if (USART1_RX_STA & (1 << 15)) {
        USART1_RX_BUF[USART1_RX_STA & ~(1 << 15)] = 0; //????????
        if (strstr((const char *) USART1_RX_BUF, str)) {
//            USART1_ClearRecvBuf();
            return SUCCESS;
        }
    }
//    USART1_ClearRecvBuf();
    return ERROR;
}

void USART1_ClearRecvBuf() {
    memset(USART1_RX_BUF, 0, USART1_RX_STA & ~(1 << 15));
    USART1_RX_STA = 0;
}

void USART1_TIMER_Config(u16 period, u16 perscaler) {
    USART1_TIMER_APBxClockCmd(USART1_TIMER_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    TIMER_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Period = period;
    TIMER_InitStructure.TIM_Prescaler = perscaler;
    TIMER_InitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(USART1_TIMERx, &TIMER_InitStructure);

    TIM_ClearFlag(USART1_TIMERx, TIM_FLAG_Update);
    TIM_ITConfig(USART1_TIMERx, TIM_IT_Update, ENABLE);
    TIM_Cmd(USART1_TIMERx, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_TIMER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(USART1_TIMERx, DISABLE);
}


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief USART1定时器中断处理函数
 */
void USART1_TIMER_IRQHandler() {
    if (TIM_GetITStatus(USART1_TIMERx, TIM_IT_Update) != RESET) {
        USART1_RX_STA |= 1 << 15;    //?????????
        TIM_ClearITPendingBit(USART1_TIMERx, TIM_IT_Update);
        TIM_Cmd(USART1_TIMERx, DISABLE);  //???TIM4
    }
}

/**
 * @brief USART1中断处理函数
 */
void USART1_IRQHandler() {
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
        u8 res = USART_ReceiveData(USART1);
        if ((USART1_RX_STA & (1 << 15)) == 0) {
            if (USART1_RX_STA < USART1_MAX_RECV_LEN) {
                TIM_SetCounter(USART1_TIMERx, 0);
                if (USART1_RX_STA == 0) {
                    TIM_Cmd(USART1_TIMERx, ENABLE);
                }
                USART1_RX_BUF[USART1_RX_STA++] = res;
            } else {
                USART1_RX_STA |= 1 << 15;
            }
        }
    }
}

#ifdef __cplusplus
}
#endif
