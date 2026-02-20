#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <WiFi.h>
#include <TimeLib.h>

#include "TimeDisplay.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;

#include "Init.h"

sensors_event_t humidity, temp;

constexpr unsigned long Time_To_UpDate = 1000 * 60 * 10;
tm updateClock() {
    struct tm timeinfo{};
    getLocalTime(&timeinfo);
    Serial2.println("Fetching Time");
    configTime(8 * 3600, 0, NTP1, NTP2,NTP3);
    Serial2.println(&timeinfo, "Time is: %F %T %A");
    return timeinfo;
}
volatile unsigned long now_mills;

void oled_Display(tm time,float Temperature,double Pressure,byte page) {
    u8g2.clearBuffer();
    if (page) {
        u8g2.setFont(u8g2_font_wqy13_t_gb2312);
        u8g2.setCursor(0, 14);
        u8g2.printf("温:%.1f*C", Temperature);
        u8g2.setCursor(0,30);
        u8g2.printf("压:%.1fkPa",Pressure);
        u8g2.setCursor(70,14);
        u8g2.printf("湿:%.1frH",humidity.relative_humidity);
        u8g2.setCursor(70,30);
        u8g2.print(millis() - now_mills);
    }else {
        u8g2.setFont(u8g2_font_fur20_tn);
        u8g2.setCursor(5,28);
        u8g2.print(&time,"%T");
    }
    u8g2.sendBuffer();
}

struct tm Timeinfo;
float Temperature = 0.0;
double Pressure = 0.0;
byte pages = 1;

void setup() {
    const unsigned long start = millis();
    Init_System();
    Init_Wifi();
    Init_Senser();
    Init_Display();
    Init_Time();
    configTime(8 * 3600,
            0,
                    NTP1,
                    NTP2,
                    NTP3);

    Timeinfo =  updateClock();

    Temperature = bmp.readTemperature();
    Pressure = bmp.readPressure() / 1000.0;
    Serial2.println("%.2f *C %.2f kPa");
    const unsigned long end = millis();
    Serial2.printf("ALL thing OK,taking %lu MS",end-start);
}

volatile byte statis = 0;

void loop() {
    static unsigned long lastNtpUpdate, lastSenserUpdate = 0;
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

    if (touchRead(1) > 100000 && statis == 0) {
         pages = !pages;
         statis = 1;
    }
    if (touchRead(1) < 100000) statis =0;

    getLocalTime(&Timeinfo);

    oled_Display(Timeinfo,Temperature,Pressure,pages); //渲染屏幕

}
