#include "esp8266.h"

void ESP8266_Config(const u8 *ssid, const u8 *pwd, ESP8266_ECN encry)
{
    USART_Config();
    TIMER_Config(1000 - 1, 7200 - 1);   //定时10ms
    while(ESP8266_Send_Cmd("AT", "OK", 20) == ERROR) //检查WIFI模块是否在线
    {
        ESP8266_Quit_Trans();//退出透传
        ESP8266_Send_Cmd("AT+CIPMODE=0", "OK", 200); //关闭透传模式
        //TODO: 提示未连接 ESP8266
    }
    while(ESP8266_Send_Cmd("ATE0", "OK", 20) == ERROR); //关闭回显
    ESP8266_Send_Cmd("AT+CWMODE=2", "OK", 50); //设置WIFI AP模式
    u8 BUF[500];
    sprintf((char *)BUF, "AT+CWSAP=%s,%s,5,%d", ssid, pwd, encry);
    ESP8266_Send_Cmd(BUF, "OK", 50);
}

u8 ESP8266_Check_Connection()
{
    u8 res;
    if(ESP8266_Quit_Trans() == ERROR)
    {
        return ERROR;    //退出透传
    }
    ESP8266_Send_Cmd("AT+CWLIF", "OK", 50);	//发送AT+CIPSTATUS指令,查询连接状态
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
    if(USART_RX_STA & 0X8000)		//接收到一次数据了
    {
        USART_RX_BUF[USART_RX_STA & 0X7FFF] = 0; //添加结束符
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
    USART_Printf("%s\r\n", cmd);	//发送命令
    if(ack && waittime)		//需要等待应答
    {
        while(--waittime)	//等待倒计时
        {
            delay_ms(10);
            if(USART_RX_STA & 0X8000) //接收到期待的应答结果
            {
                if(ESP8266_Check_Cmd(ack))
                {
                    res = SUCCESS;
                    break;//得到有效数据
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
    while((USARTx->SR & 0X40) == 0);	//等待发送空
    USARTx->DR = '+';
    delay_ms(15);					//大于串口组帧时间(10ms)
    while((USARTx->SR & 0X40) == 0);	//等待发送空
    USARTx->DR = '+';
    delay_ms(15);					//大于串口组帧时间(10ms)
    while((USARTx->SR & 0X40) == 0);	//等待发送空
    USARTx->DR = '+';
    delay_ms(500);					//等待500ms
    return ESP8266_Send_Cmd("AT", "OK", 20); //退出透传判断.
}