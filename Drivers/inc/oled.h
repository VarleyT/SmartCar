//
// Created by 19634 on 2023/8/14.
//

#ifndef __OLED_H
#define __OLED_H

#include "sys.h"
#include "delay.h"
#include "string.h"
#include "oledlib.h"

#define OLED_GPIO_CMD           RCC_APB2PeriphClockCmd
#define OLED_GPIO_RCC           RCC_APB2Periph_GPIOB
#define OLED_GPIOx              GPIOB
#define OLED_SCL_Pin            GPIO_Pin_10
#define OLED_SDA_Pin            GPIO_Pin_11
#define OLED_IIC_CMD            RCC_APB1PeriphClockCmd
#define OLED_IIC_RCC            RCC_APB1Periph_I2C2
#define OLED_IICx               I2C2

#define OLED_IIC_ClkSpeed       100000
#define OLED_IIC_OwnAddr        0x5F

#define OLED_IIC_WRITE          0
#define OLED_IIC_READ           1
#define OLED_IIC_WRITE_DATA     0x40
#define OLED_IIC_WRITE_CMD      0x00
#define OLED_IIC_ADDR           0x3C

#define OLED_PAGES              8
#define OLED_WIDTH              128
#define OLED_HEIGHT             64


class OLED {

public:
    OLED();

    void clear();

    void setPtrPos(u8 x, u8 y);

    void showChar(u8 x, u8 y, u8 ch);

    void showChinese(u8 x, u8 y, const char *ch);

    void showString(u8 x, u8 y, const char *str);

    void drawIcon(u8 x, u8 y, u8 width, u8 height, const u8 icon[]);
};


#endif //__OLED_H
