#include "timer.h"

//定时器4中断服务程序
void TIMERx_IRQHandler()
{
    if(TIM_GetITStatus(TIMERx, TIM_IT_Update) != RESET) //是更新中断
    {
        USART_RX_STA |= 1 << 15;	//标记接收完成
        TIM_ClearITPendingBit(TIMERx, TIM_IT_Update);    //清除TIM4更新中断标志
        TIM_Cmd(TIMERx, DISABLE);  //关闭TIM4
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    TIM_Cmd(TIMERx, DISABLE);
}