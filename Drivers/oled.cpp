//
// Created by 19634 on 2023/8/14.
//

#include "oled.h"

static void OLED_WriteCmd(const u8 cmd) {
    I2C_GenerateSTART(I2C2, ENABLE);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);
    I2C_Send7bitAddress(I2C2, ((OLED_IIC_ADDR << 1)+ OLED_IIC_WRITE), I2C_Direction_Transmitter);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ==
           ERROR);   //确认选择地址
    I2C_SendData(I2C2, OLED_IIC_WRITE_CMD);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);   //确认发送数据
    I2C_SendData(I2C2, cmd);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);    //确认发送完成
    I2C_GenerateSTOP(I2C2, ENABLE);
}

static void OLED_WriteData(const u8 data) {
    I2C_GenerateSTART(I2C2, ENABLE);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) == ERROR);     //确认发送
    I2C_Send7bitAddress(I2C2, ((OLED_IIC_ADDR << 1)+ OLED_IIC_WRITE), I2C_Direction_Transmitter);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) ==
           ERROR);   //确认选择地址
    I2C_SendData(I2C2, OLED_IIC_WRITE_DATA);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR);   //确认发送数据
    I2C_SendData(I2C2, data);
    while (I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR);    //确认发送完成
    I2C_GenerateSTOP(I2C2, ENABLE);
}

static void OLED_DisplayOff() {
    OLED_WriteCmd(0XAE);    //禁用显示
    OLED_WriteCmd(0X8D);    //充电泵设置
    OLED_WriteCmd(0X10);    //启用充电泵
}

static void OLED_DisplayOn() {
    OLED_WriteCmd(0X8D);    //充电泵设置
    OLED_WriteCmd(0X14);    //启用充电泵
    OLED_WriteCmd(0XAF);    //启用显示
}

static void OLED_Init() {
    Delay_ms(250);
    OLED_DisplayOff();
    OLED_WriteCmd(0x00); //---set low column address
    OLED_WriteCmd(0x10); //---set high column address
    OLED_WriteCmd(0x40); //--set start line address
    OLED_WriteCmd(0xB0); //--set page address
    OLED_WriteCmd(0x81); // contract control
    OLED_WriteCmd(0xFF); //--128
    OLED_WriteCmd(0xA1); //set segment remap
    OLED_WriteCmd(0xA6); //--normal / reverse
    OLED_WriteCmd(0xA8); //--set multiplex ratio(1 to 64)
    OLED_WriteCmd(0x3F); //--1/32 duty
    OLED_WriteCmd(0xC8); //Com scan direction
    OLED_WriteCmd(0xD3); //-set display offset
    OLED_WriteCmd(0x00); //

    OLED_WriteCmd(0xD5); //set osc division
    OLED_WriteCmd(0x80); //

    OLED_WriteCmd(0xD8); //set area color mode off
    OLED_WriteCmd(0x05); //

    OLED_WriteCmd(0xD9); //Set Pre-Charge Period
    OLED_WriteCmd(0xF1); //

    OLED_WriteCmd(0xDA); //set com pin configuartion
    OLED_WriteCmd(0x12); //

    OLED_WriteCmd(0xDB); //set Vcomh
    OLED_WriteCmd(0x30); //
    OLED_DisplayOn();
}

/**
 * @brief OLED Driver for SSD1315
 */
OLED::OLED() {
    //  GPIO初始化
    OLED_GPIO_CMD(OLED_GPIO_RCC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_Pin | OLED_SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(OLED_GPIOx, &GPIO_InitStructure);
    //  I2C初始化
    OLED_IIC_CMD(OLED_IIC_RCC, ENABLE);
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed = OLED_IIC_ClkSpeed;
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = OLED_IIC_OwnAddr;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(OLED_IICx, &I2C_InitStructure);
    //  使能I2C
    I2C_Cmd(OLED_IICx, ENABLE);
    OLED_Init();
}

void OLED::setPtrPos(u8 x, u8 y) {
    OLED_WriteCmd(0xB0 + (y <= 7 ? y : 7));    //设置页面地址
    OLED_WriteCmd((((x <= 127 ? x : 127) & 0xF0) >> 4) | 0x10);   //设置列高位地址
    OLED_WriteCmd(x & 0x0F);   //设置列低位地址
}

void OLED::clear() {
    u8 i, j;
    for (i = 0; i < OLED_PAGES; ++i) {
        setPtrPos(0, i);
        for (j = 0; j < OLED_WIDTH; ++j) {
            OLED_WriteData(0xFF);
        }
    }
}

void OLED::showChar(u8 x, u8 y, u8 ch) {
    y = y <= (OLED_PAGES - 1) - 1 ? y : (OLED_PAGES - 1) - 1;  //保证能完全显示
    u8 c, i;
    c = ch - ' '; //得到偏移后的值
    setPtrPos(x, y);
    for (i = 0; i < 8; i++) {
        OLED_WriteData(~OLED_FONT_F8X16[c * 16 + i]);
    }
    setPtrPos(x, y + 1);
    for (i = 0; i < 8; i++) {
        OLED_WriteData(~OLED_FONT_F8X16[c * 16 + i + 8]);
    }
}

void OLED::showChinese(u8 x, u8 y, const char *ch) {
    y = y <= (OLED_PAGES - 1) - 1 ? y : (OLED_PAGES - 1) - 1;  //保证能完全显示
    u8 c = 0xFF, i;
    for (i = 0; i < (strlen(OLED_FONT_CHN_LIBRARY) / 2); ++i) {
        if (OLED_FONT_CHN_LIBRARY[i] == *ch) {
            c = i;
        }
    }
    if (c != 0xFF) {
        setPtrPos(x, y);
        for (i = 0; i < 16; i++) {
            OLED_WriteData(~OLED_FONT_CHN_MATRIX[c * 32 + i]);
        }
        setPtrPos(x, y + 1);
        for (i = 0; i < 16; i++) {
            OLED_WriteData(~OLED_FONT_CHN_MATRIX[c * 32 + i + 16]);
        }
    } else {
        showChar(x, y, ' ');
    }
}

void OLED::showString(u8 x, u8 y, const char *str) {
    u8 i = x;
    while (*str != '\0') {
        if ((*str >= 0 && *str <= 127)) {
            showChar(i, y, *str);
            i += 8;
        } else if (*str >= 0xa1 && *str <= 0xfe)       //中文
        {
            showChinese(i, y, str);
            str++;
            i += 16;
        }
        str++;
    }
}

void OLED::drawIcon(u8 x, u8 y, u8 width, u8 height, const u8 icon[]) {
    u8 i, j, pages;
    pages = ((height % 8) ? height + 8 : height) / 8;
    for (j = 0; j < pages; ++j) {
        setPtrPos(x, y + j);
        for (i = 0; i < width; ++i) {
            OLED_WriteData(~icon[width * j + i]);
        }
    }
}
