#include "sys.h"
#include "motor.h"
#include "esp8266.h"
#include "whistle.h"

#define SSID           "\"ESP8266\""
#define PASSWD         "\"12345678\""

void response()
{
    if(USART_RX_STA & 0X8000)		//接收到一次数据了
    {
        USART_RX_BUF[USART_RX_STA & 0X7FFF] = 0; //添加结束符
        if(ESP8266_Check_Cmd("forward") == SUCCESS)
        {
            MOTOR_SetDirection(MOTOR_DIRECTION_FORWARD);
        }
        else if(ESP8266_Check_Cmd("backward") == SUCCESS)
        {
            MOTOR_SetDirection(MOTOR_DIRECTION_BACKWARD);
        }
        else if(ESP8266_Check_Cmd("left") == SUCCESS)
        {
            MOTOR_SetDirection(MOTOR_DIRECTION_LEFT);
        }
        else if(ESP8266_Check_Cmd("right") == SUCCESS)
        {
            MOTOR_SetDirection(MOTOR_DIRECTION_RIGHT);
        }
        else if(ESP8266_Check_Cmd("stop") == SUCCESS)
        {
            MOTOR_SetDirection(MOTOR_DIRECTION_STOP);
        }
        USART_RX_STA = 0;
    }
}

int main()
{
    delay_init();
    ESP8266_Config(SSID, PASSWD, ESP8266_ECN_WPA2_PSK);
    MOTOR_Config();
    WHISTLE_Config();
    
    while(ESP8266_Check_Connection() != SUCCESS)
    {delay_ms(2000);}    //等待接入WiFi
    while(ESP8266_Connect_TCP() != SUCCESS)
    {delay_ms(2000);}    //等待连接至TCP服务器
    
    ESP8266_Send_Cmd("AT+CIPMODE=1", "OK", 20);
    ESP8266_Send_Cmd("AT+CIPSEND", "OK", 200);      //开启透传
    
    USART_RX_STA = 0;
    WHISTLE_Tip();
    
    while(1)
    {
        response();
    }
    return 0;
}