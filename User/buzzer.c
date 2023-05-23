#include "buzzer.h"

void BUZZER_Config()
{
    RCC_APB2PeriphClockCmd(BUZZER_CLK, ENABLE);
    
    GPIO_InitTypeDef typeDef;
    typeDef.GPIO_Mode = GPIO_Mode_Out_PP;
    typeDef.GPIO_Pin = BUZZER_GPIO_Pin;
    typeDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_PORT, &typeDef);
}

void BUZZER_PlayNormalSound()
{
    GPIO_SetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_NORMAL_DURATION);           
    GPIO_ResetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_NORMAL_DURATION);          
    
    GPIO_SetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_NORMAL_DURATION);           
    GPIO_ResetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_NORMAL_DURATION);   
}

void BUZZER_PlayErrorSound()
{
    GPIO_SetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION);           
    GPIO_ResetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION);          
    
    GPIO_SetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION);           
    GPIO_ResetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION);  
    
    GPIO_SetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION);           
    GPIO_ResetBits(BUZZER_PORT, BUZZER_GPIO_Pin); 
    Delay_ms(BUZZER_ERROR_DURATION); 
}