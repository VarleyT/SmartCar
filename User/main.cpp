#include "sys.h"
#include "stdlib.h"
#include "delay.h"
#include "motor.h"
#include "esp8266.h"
#include "buzzer.h"
#include "oled.h"
#include "dht11.h"
#include "led.h"

#define SSID           "\"SmartCar\""

ESP8266 esp8266(SSID);
MOTOR motor;
OLED oled;
LED led_r(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_4);
LED led_g(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_5);
LED led_b(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_6);
BUZZER buzzer(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_7);
DHT11 dht11(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_8);

void convertCommand();

int main() {
    u16 count = 0;
    oled.clear();
    oled.showString(32, 3, "disconnect");
    oled.drawIcon(0, 0, 16, 16, OLED_ICON_WIFI_DISCONNECTED);
    oled.drawIcon(0 + 16 + 4, 0, 16, 16, OLED_ICON_TCP_DISCONNECTED);
    while (esp8266.checkConnection() == ERROR) Delay_s(2);
    oled.drawIcon(0, 0, 16, 16, OLED_ICON_WIFI_CONNECTED);
    while (esp8266.connect2Service() == ERROR) Delay_s(2);
    oled.drawIcon(0 + 16 + 4, 0, 16, 16, OLED_ICON_TCP_CONNECTED);
    oled.showString(32, 3, "          ");
    oled.showString(4, 2, "temp: --.- C");
    oled.showString(4, 4, "humi: --.- %");
    esp8266.enterTrans();
    while (true) {
        convertCommand();
        if (count == 5000) {    // 5s更新一次温湿度
            DHT11Data_Struct data = dht11.getData();
            if (data.temperature != 0.0) {
                char buf[6];
                sprintf(buf, "%.1f", data.temperature);
                oled.showString(4 + 16 * 3, 2, buf);
                sprintf(buf, "%.1f", data.humidity);
                oled.showString(4 + 16 * 3, 4, buf);
            }
            count = 0;
        }
        count++;
        Delay_ms(1);
    }
    return 0;
}

void convertCommand(){
    if (USART1_RX_STA & (1 << 15)) {
        if (strstr((const char *) USART1_RX_BUF, "direction")) {
            const char *p = strstr((const char *) USART1_RX_BUF, "direction=");
            p++;
            if (strstr(p, "stop")) {
                motor.setDirection(MOTOR_DIRECTION_STOP);
            } else if (strstr(p, "forward")) {
                motor.setDirection(MOTOR_DIRECTION_FORWARD);
            } else if (strstr(p, "backward")) {
                motor.setDirection(MOTOR_DIRECTION_BACKWARD);
            } else if (strstr(p, "left")) {
                motor.setDirection(MOTOR_DIRECTION_LEFT);
            } else if (strstr(p, "right")) {
                motor.setDirection(MOTOR_DIRECTION_RIGHT);
            }
        }
        if (strstr((const char *) USART1_RX_BUF, "pwm")) {
            const char *num1 = strstr((const char *) USART1_RX_BUF, "pwm=");
            const char *num2 = strchr(num1, ',');
            motor.setSpeed(atoi(num1 + 1), atoi(num2 + 1));
        }
        if (strstr((const char *) USART1_RX_BUF, "light")) {
            const char *ch = strstr((const char *) USART1_RX_BUF, "light=");
            if (atoi(ch + 1)) {
                led_r.on();
                led_g.on();
                led_b.on();
            } else {
                led_r.off();
                led_g.off();
                led_b.off();
            }
        }
        if (strstr((const char *) USART1_RX_BUF, "buzzer")) {
            const char *ch = strstr((const char *) USART1_RX_BUF, "buzzer=");
            if (atoi(ch + 1)) buzzer.on(); else buzzer.off();
        }
        USART1_ClearRecvBuf();
    }
}
