//
// Created by loogze on 2026/2/19.
//

#ifndef UNTITLED_INIT_H
#define UNTITLED_INIT_H

#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include <Wire.h>
#include "parvite.h"

#define NTP1  "ntp1.aliyun.com"
#define NTP2  "ntp2.aliyun.com"
#define NTP3  "ntp3.aliyun.com"
#define ON_BOARD_LED 48


constexpr int rxPin = 16;
constexpr int txPin = 17;

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);
Adafruit_BMP280 bmp;
Adafruit_AHTX0 aht;
sensors_event_t humidity, temp;

volatile float Temperature = 0.0;
volatile double Pressure = 0.0;

inline void Init_System() {
    Serial.begin(9600);
    Serial2.begin(115200,SERIAL_8N1,rxPin,txPin);
    pinMode(ON_BOARD_LED,OUTPUT);
    Serial2.println("Sys OK");
}

inline void Init_Senser() {
    if (!bmp.begin()) {
        Serial2.printf("Fail to start BMP20");
        while (true) {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_wqy12_t_gb2312);
            u8g2.setCursor(24,20);
            u8g2.println("无法初始化BMP280，检查连线");
            u8g2.sendBuffer();
        }
    }
    if (! aht.begin()) {
        Serial2.println("Could not find AHT? Check wiring");
        while (true) {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_wqy12_t_gb2312);
            u8g2.setCursor(24,20);
            u8g2.println("无法初始化AHT20，检查连线");
            u8g2.sendBuffer();
        }
    }
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16, /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500);
    Temperature = bmp.readTemperature();
    Pressure = bmp.readPressure() / 1000.0; // 默认千帕
    Serial2.printf("Temp is: %.2f *C  Pressure is: %.2f kPa",Temperature,Pressure);
    Serial2.println();
    Serial2.println("Senser OK");

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_wqy12_t_gb2312);
    u8g2.setCursor(24,20);
    u8g2.println("传感器初始化完成");
    u8g2.sendBuffer();
}

inline void Init_Wifi() {
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
    Serial2.printf("WIFI %s Connecting",ssid);
    int counter = 0;
    while (WiFiClass::status() != WL_CONNECTED) {
        static unsigned long now = 0;
        if (millis() - now >= 300) {
            digitalWrite(ON_BOARD_LED,!digitalRead(ON_BOARD_LED));
            now = millis();
            counter++;
            Serial2.print(".");
        }
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_unifont_t_gb2312b);
        u8g2.setCursor(24,20);
        u8g2.println("连接WIFI");
        u8g2.sendBuffer();
        if (counter > 34) break;
    }
    Serial2.println();
    Serial2.printf("WiFi connected. IP:");
    Serial2.print(WiFi.localIP());
    Serial2.println("");
}

inline void Init_Display() {
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_unifont_t_gb2312b);
    u8g2.setCursor(24,20);
    u8g2.println("欢迎");
    u8g2.sendBuffer();
    Serial2.println("U8G2 OK");
}


#endif //UNTITLED_INIT_H