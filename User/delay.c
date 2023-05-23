#include "delay.h"

u32 TimingDelay;

void Delay_Config()
{
    //  SystemCoreClock / 1000      1ms�ж�
    //  SystemCoreClock / 10000     10us�ж�
    //  SystemCoreClock / 100000    1us�ж�
    if(SysTick_Config(SystemCoreClock / 1000))
    {
        // TODO: ��ʼ��ϵͳ�δ�ʱ���쳣
    }
}

void Delay_ms(u16 nms)
{
    TimingDelay = nms;
    while(TimingDelay);
}

void SysTick_Handler()
{
    if(TimingDelay != 0)
    {
        TimingDelay--;
    }
}