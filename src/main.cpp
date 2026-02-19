#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R2);
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;

auto ssid = "TP-link_A67A";
auto passwd = "0852wppu,.";

#define FONT u8g2_font_wqy12_t_chinese2
#define ON_BOARD_LED 48

#define NTP1  "ntp1.aliyun.com"
#define NTP2  "ntp2.aliyun.com"
#define NTP3  "ntp3.aliyun.com"

#include "Init.h"



sensors_event_t humidity, temp;

struct tm current_time;

void oled_Display() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_mf);
    u8g2.setCursor(0, 10);
    u8g2.printf("Temperature: %.1f *C", bmp.readTemperature());
    u8g2.setCursor(0,21);
    u8g2.printf("Pressure: %.1f kPa",bmp.readPressure()/1000.0);
    u8g2.setCursor(0,32);
    u8g2.printf("Humidity: %.1f rH",humidity.relative_humidity);
    u8g2.sendBuffer();
}

void setup() {
    Init_Display();
    Init_Senser();
    Init_Wifi();
    Init_Time();
    configTime(8 * 3600,
            0,
                    NTP1,
                    NTP2,
                    NTP3);
    updateClock();
}

void loop() {
    aht.getEvent(&humidity, &temp); // 获取湿度

    static unsigned long lastNtpUpdate = 0;
    unsigned long now = millis();
    if (now - lastNtpUpdate >= 60000) {
        updateClock();
        lastNtpUpdate = now;
    }


    oled_Display();//渲染屏幕

    delay(100);
}
