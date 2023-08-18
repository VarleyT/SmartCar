//
// Created by 19634 on 2023/8/11.
//

#ifndef __ESP8266_H
#define __ESP8266_H

#include "sys.h"
#include "usart1.h"
#include "delay.h"

typedef enum {
    ESP8266_ECN_OPEN = 0,
    ESP8266_ECN_WPA_PSK = 2,
    ESP8266_ECN_WPA2_PSK = 3,
    ESP8266_ECN_WPA_WPA2_PSK = 4
} ESP8266_ECN;

class ESP8266 {
public:
    ESP8266();

    ESP8266(const char *ssid, const char *pwd = "", ESP8266_ECN encry = ESP8266_ECN_OPEN);

    u8 checkConnection();

    u8 connect2Service();

    u8 checkService();

    u8 enterTrans();

    u8 quitTrans();

private:
    u8 sendCmd(const char *cmd, const char *ack = "", u16 waitTime = 0);
};


#endif //__ESP8266_H
