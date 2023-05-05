#include "timer.h"

//��ʱ��4�жϷ������
void TIMERx_IRQHandler()
{
    if(TIM_GetITStatus(TIMERx, TIM_IT_Update) != RESET) //�Ǹ����ж�
    {
        USART_RX_STA |= 1 << 15;	//��ǽ������
        TIM_ClearITPendingBit(TIMERx, TIM_IT_Update);    //���TIM4�����жϱ�־
        TIM_Cmd(TIMERx, DISABLE);  //�ر�TIM4
    }
}

void TIMER_Config(u16 period, u16 perscaler)
{
    TIMER_APBxClockCmd(TIMER_CLK, ENABLE);

    TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    TIMER_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Period = period;
    TIMER_InitStructure.TIM_Prescaler = perscaler;
    TIMER_InitStructure.TIM_RepetitionCounter = 0;

    TIM_TimeBaseInit(TIMERx, &TIMER_InitStructure);

    TIM_ClearFlag(TIMERx, TIM_FLAG_Update);
    TIM_ITConfig(TIMERx, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIMERx, ENABLE);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //��ռ���ȼ�0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    TIM_Cmd(TIMERx, DISABLE);
}