#include "delay.h"

u32 TimingDelay;

void Delay_Config()
{
    //  SystemCoreClock / 1000      1ms中断
    //  SystemCoreClock / 10000     10us中断
    //  SystemCoreClock / 100000    1us中断
    if(SysTick_Config(SystemCoreClock / 1000))
    {
        // TODO: 初始化系统滴答定时器异常
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