#ifndef __ESP8266_H
#define __ESP8266_H

#include "usart.h"
#include "timer.h"
#include "delay.h"

typedef enum
{
    ESP8266_ECN_OPEN = 0,
    ESP8266_ECN_WPA_PSK = 2,
    ESP8266_ECN_WPA2_PSK = 3,
    ESP8266_ECN_WPA_WPA2_PSK = 4
}ESP8266_ECN;

void ESP8266_Config(const u8 *ssid, const u8 *pwd, ESP8266_ECN encry);

u8 ESP8266_Check_Connection(void);

u8 ESP8266_Connect_TCP(void);

u8 ESP8266_Check_Cmd(u8 *str);

u8 ESP8266_Send_Cmd(u8 *cmd, u8 *ack, u16 waittime);

u8 ESP8266_Quit_Trans(void);

#endif