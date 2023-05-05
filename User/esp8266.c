#include "esp8266.h"

void ESP8266_Config(const u8 *ssid, const u8 *pwd, ESP8266_ECN encry)
{
    USART_Config();
    TIMER_Config(1000 - 1, 7200 - 1);   //��ʱ10ms
    while(ESP8266_Send_Cmd("AT", "OK", 20) == ERROR) //���WIFIģ���Ƿ�����
    {
        ESP8266_Quit_Trans();//�˳�͸��
        ESP8266_Send_Cmd("AT+CIPMODE=0", "OK", 200); //�ر�͸��ģʽ
        //TODO: ��ʾδ���� ESP8266
    }
    while(ESP8266_Send_Cmd("ATE0", "OK", 20) == ERROR); //�رջ���
    ESP8266_Send_Cmd("AT+CWMODE=2", "OK", 50); //����WIFI APģʽ
    u8 BUF[500];
    sprintf((char *)BUF, "AT+CWSAP=%s,%s,5,%d", ssid, pwd, encry);
    ESP8266_Send_Cmd(BUF, "OK", 50);
}

u8 ESP8266_Check_Connection()
{
    u8 res;
    if(ESP8266_Quit_Trans() == ERROR)
    {
        return ERROR;    //�˳�͸��
    }
    ESP8266_Send_Cmd("AT+CWLIF", "OK", 50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
    return ESP8266_Check_Cmd("192.168.1.2");
}

u8 ESP8266_Connect_TCP()
{
    ESP8266_Send_Cmd("AT+CIPSTART=\"TCP\",\"192.168.1.2\",8000", "OK", 50);
    return ESP8266_Check_Cmd("CONNECT");
}

u8 ESP8266_Check_Cmd(u8 *str)
{
    char *strx = 0;
    if(USART_RX_STA & 0X8000)		//���յ�һ��������
    {
        USART_RX_BUF[USART_RX_STA & 0X7FFF] = 0; //��ӽ�����
        strx = strstr((const char *)USART_RX_BUF, (const char *)str);
        if(strx)
        {
            return SUCCESS;
        }
    }
    return ERROR;
}

u8 ESP8266_Send_Cmd(u8 *cmd, u8 *ack, u16 waittime)
{
    //memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
    u8 res = ERROR;
    USART_RX_STA = 0;
    USART_Printf("%s\r\n", cmd);	//��������
    if(ack && waittime)		//��Ҫ�ȴ�Ӧ��
    {
        while(--waittime)	//�ȴ�����ʱ
        {
            delay_ms(10);
            if(USART_RX_STA & 0X8000) //���յ��ڴ���Ӧ����
            {
                if(ESP8266_Check_Cmd(ack))
                {
                    res = SUCCESS;
                    break;//�õ���Ч����
                }
                USART_RX_STA = 0;
            }
        }
        if(waittime == 0)
        {
            res = ERROR;
        }
    }
    return res;
}

u8 ESP8266_Quit_Trans()
{
    while((USARTx->SR & 0X40) == 0);	//�ȴ����Ϳ�
    USARTx->DR = '+';
    delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
    while((USARTx->SR & 0X40) == 0);	//�ȴ����Ϳ�
    USARTx->DR = '+';
    delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
    while((USARTx->SR & 0X40) == 0);	//�ȴ����Ϳ�
    USARTx->DR = '+';
    delay_ms(500);					//�ȴ�500ms
    return ESP8266_Send_Cmd("AT", "OK", 20); //�˳�͸���ж�.
}