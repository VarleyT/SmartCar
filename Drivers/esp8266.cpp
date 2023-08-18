//
// Created by 19634 on 2023/8/11.
//

#include "esp8266.h"


ESP8266::ESP8266() = default;

/**
 * @brief 初始化ESP8266模块
 * @param ssid WIFI名称
 * @param pwd WIFI密码
 * @param encry 安全性
 */
ESP8266::ESP8266(const char *ssid, const char *pwd, ESP8266_ECN encry) {
    USART1_Config();
    while (sendCmd("AT", "OK", 20) == ERROR) //检查WIFI模块是否在线
    {
        quitTrans();//退出透传
        sendCmd("AT+CIPMODE=0", "OK", 200); //关闭透传模式
        //TODO: 提示未连接 ESP8266
    }
    while (sendCmd("ATE0", "OK", 50) == ERROR); //关闭回显
    while (sendCmd("AT+CWMODE=2", "OK", 50) == ERROR); //设置WIFI AP模式
    char BUF[500];
    sprintf(BUF, "AT+CWSAP=%s,%s,5,%d", ssid, pwd, encry);
    while (sendCmd((const char *) BUF, "OK", 50) == ERROR);  //设置WIFI基本信息
}

/**
 * @brief 发送命令到串口
 * @param cmd 命令内容
 * @param ack 期待应答
 * @param waitTime 响应时间 单位: 10ms
 * @return SUCCESS | ERROR
 */
u8 ESP8266::sendCmd(const char *cmd, const char *ack, u16 waitTime) {
    USART1_Printf("%s\r\n", cmd);    //发送命令
    if (ack && waitTime)        //需要等待应答
    {
        while (--waitTime)    //等待倒计时
        {
            Delay_ms(10);
            if (USART1_CheckData(ack) == SUCCESS) {
                return SUCCESS;
            }
        }
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief 进入透传模式
 * @return SUCCESS | ERROR
 */
u8 ESP8266::enterTrans() {
    if (sendCmd("AT+CIPMODE=1", "OK", 20)) {
        sendCmd("AT+CIPSEND");
        Delay_ms(20);
        return SUCCESS;
    }
    return ERROR;
}

/**
 * @brief 退出透传模式
 * @return SUCCESS | ERROR
 */
u8 ESP8266::quitTrans() {
    while ((USART1->SR & 0X40) == 0);    //等待发送空
    USART1->DR = '+';
    Delay_ms(15);                    //大于串口组帧时间(10ms)
    while ((USART1->SR & 0X40) == 0);    //等待发送空
    USART1->DR = '+';
    Delay_ms(15);                    //大于串口组帧时间(10ms)
    while ((USART1->SR & 0X40) == 0);    //等待发送空
    USART1->DR = '+';

    Delay_ms(500);                    //等待500ms
    return sendCmd("AT", "OK", 20); //退出透传判断.
}

/**
 * @brief 连接至TCP服务端
 *
 * Tips: 由于ESP8266内存限制，只能作为客户端
 * @return SUCCESS | ERROR
 */
u8 ESP8266::connect2Service() {
    sendCmd(R"(AT+CIPSTART="TCP","192.168.1.2",8000)", "OK", 50);
    return USART1_CheckData("CONNECT") | USART1_CheckData("ALREADY CONNECTED");
}

/**
 * @brief 检查TCP连接状态
 * @return SUCCESS | ERROR
 */
u8 ESP8266::checkService() {
    if (quitTrans() == SUCCESS) {
        return sendCmd("AT+CIPSTATUS", "192.168.1.2", 50);
    }
    return ERROR;
}

/**
 * @brief 查询设备WIFI连接情况
 * @return SUCCESS | ERROR
 */
u8 ESP8266::checkConnection() {
    if (quitTrans() == SUCCESS) {
        return sendCmd("AT+CWLIF", "192.168.1.2", 100);
    }
    return ERROR;
}
