#include "sys.h"
#include "motor.h"
#include "esp8266.h"
#include "buzzer.h"

#define SSID           "\"ESP8266\""
#define PASSWD         "\"12345678\""

u8 state = SUCCESS;

void Start_ESP8266()
{
    while(ESP8266_Check_Connection() != SUCCESS)
    {Delay_ms(2000);}    //等待接入WiFi
    while(ESP8266_Connect_TCP() != SUCCESS)
    {Delay_ms(2000);}    //等待连接至TCP服务器
    
    BUZZER_PlayNormalSound();
}

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
        }else if(ESP8266_Check_Cmd("pwm") == SUCCESS)
        {
            u8 *ch = strchr(USART_RX_BUF, '=');
            if(ch != NULL)
            {
                MOTOR_SetSpeed(atoi(ch+1));
            }
        }
        USART_RX_STA = 0;
    }
}

int main()
{
    Delay_Config();
    ESP8266_Config(SSID, PASSWD, ESP8266_ECN_WPA2_PSK);
    MOTOR_Config();
    BUZZER_Config();

    Start_ESP8266();
    
    u8 count = 0;
    while(1)
    {
        if(count++ == 5)
        {
            count = 0;
            while(ESP8266_Send_Cmd("AT+CWLIF", "192.168.1.2", 50) != SUCCESS)
            {
                state = ERROR;
                BUZZER_PlayErrorSound();
                Delay_ms(5000);
            }
            if(state == ERROR)
            {
                BUZZER_PlayNormalSound();
                state = SUCCESS;
            }
        }
        response();
    }
    return 0;
}