#include "usart.h"

u8 USART_RX_BUF[USART_MAX_RECV_LEN];
u8 USART_TX_BUF[USART_MAX_SEND_LEN];
u16 direction;
vu16 USART_RX_STA = 0;

void USART_Config()
{
    USART_GPIO_APBxClkCmd(USART_GPIO_CLK, ENABLE);
    USART_APBxClkCmd(USART_CLK, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //GPIO_TX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = USART_GPIO_TX_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);
    //GPIO_RX
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = USART_GPIO_RX_Pin;
    GPIO_Init(USART_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = USART_BAUDRATE;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_Init(USARTx, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);
    USART_RX_STA = 0;
    USART_Cmd(USARTx, ENABLE);
}

void USART_IRQHandler()
{
    u8 res;
    if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)//���յ�����
    {
        res = USART_ReceiveData(USARTx);
        if((USART_RX_STA & (1 << 15)) == 0) //�������һ������,��û�б�����,���ٽ�����������
        {
            if(USART_RX_STA < USART_MAX_RECV_LEN)	//�����Խ�������
            {
                TIM_SetCounter(TIMERx, 0); //���������
                if(USART_RX_STA == 0) 				//ʹ�ܶ�ʱ��7���ж�
                {
                    TIM_Cmd(TIMERx, ENABLE); //ʹ�ܶ�ʱ��7
                }
                USART_RX_BUF[USART_RX_STA++] = res;	//��¼���յ���ֵ
            }
            else
            {
                USART_RX_STA |= 1 << 15;				//ǿ�Ʊ�ǽ������
            }
        }
    }
}

void USART_Printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART_TX_BUF, fmt, ap);
    va_end(ap);
    i = strlen((const char *)USART_TX_BUF);       //�˴η������ݵĳ���
    for(j = 0; j < i; j++)                        //ѭ����������
    {
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������
        USART_SendData(USARTx, USART_TX_BUF[j]);
    }
}