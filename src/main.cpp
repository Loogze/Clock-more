#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <TimeLib.h>

#include "Nixie.h"
#include "../include/timing.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;

#include "Init.h"

sensors_event_t humidity, temp;

const unsigned long Time_To_UpDate = 1000 * 60 * 10;

volatile unsigned long now_mills;

struct tm Timeinfo;
float Temperature = 0.0;
double Pressure = 0.0;
byte switcher = 1;
volatile byte switcher_statis = 0;

void oled_Display(tm time, byte page, unsigned long durin);

void setup() {
    const unsigned long start = millis();
    Init_System();
    Init_Display();
    Init_Senser();
    Init_Wifi();
    Init_Time();
    configTime(8 * 3600,
               0,
               NTP1,
               NTP2,
               NTP3);

    Timeinfo = updateClock();

    Temperature = bmp.readTemperature();
    Pressure = bmp.readPressure() / 1000.0; // 默认千帕
    Serial2.printf("Temp is: %.2f *C  Pressure is: %.2f kPa",Temperature,Pressure);
    Serial2.println();
    Serial2.printf("ALL thing OK,taking %lu MS", millis() - start);
}


void loop() {
    static unsigned long lastNtpUpdate, lastSenserUpdate = 0;
    static unsigned long spend_time;
    now_mills = millis();

    if (now_mills - lastNtpUpdate >= Time_To_UpDate) {
        Timeinfo = updateClock();
        Serial2.println("Updated Time");
        lastNtpUpdate = now_mills;
    }
    if (now_mills - lastSenserUpdate >= 100) {
        aht.getEvent(&humidity, &temp);
        Temperature = bmp.readTemperature();
        Pressure = bmp.readPressure() / 1000.0;
        lastSenserUpdate = now_mills;
    }

    if (touchRead(1) > 100000 && switcher_statis == 0) {
        switcher = !switcher;
        switcher_statis = 1;
    }
    if (touchRead(1) < 100000) switcher_statis = 0;

    getLocalTime(&Timeinfo);

    oled_Display(Timeinfo, switcher, spend_time); //渲染屏幕
    spend_time = millis() - now_mills;
}

void oled_Display(tm time, const byte page,const unsigned long durin) {
    u8g2.clearBuffer();
    if (page) {
        u8g2.setFont(u8g2_font_wqy13_t_gb2312);
        u8g2.setCursor(0, 14);
        u8g2.printf("温:%.1f*C", Temperature);
        u8g2.setCursor(0, 30);
        u8g2.printf("压:%.1fkPa", Pressure);
        u8g2.setCursor(70, 14);
        u8g2.printf("湿:%.1frH", humidity.relative_humidity);
        u8g2.setCursor(70, 30);
        u8g2.printf("%ld %f", durin, 1.0 / (durin / 1000.0));
    } else {
        u8g2.setFont(u8g2_font_fur20_tn);
        u8g2.setCursor(5, 28);
        u8g2.print(&time, "%T");
    }
    u8g2.sendBuffer();
}
