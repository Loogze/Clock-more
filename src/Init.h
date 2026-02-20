//
// Created by loogze on 2026/2/19.
//

#ifndef UNTITLED_INIT_H
#define UNTITLED_INIT_H

#define NTP1  "ntp1.aliyun.com"
#define NTP2  "ntp2.aliyun.com"
#define NTP3  "ntp3.aliyun.com"
#define FONT u8g2_font_wqy12_t_chinese2
#define ON_BOARD_LED 48

constexpr auto ssid = "TP-LINK_A67A";
constexpr auto passwd = "0852wppu,.";
constexpr auto rxPin = 16;
constexpr auto txPin = 17;

inline void Init_System() {
    Serial.begin(9600);
    Serial2.begin(9600,SERIAL_8N1,rxPin,txPin);
    pinMode(ON_BOARD_LED,OUTPUT);
    Serial2.println("Sys OK");
}

inline void Init_Senser() {
    if (!bmp.begin()) {
        Serial2.printf("Fail to start BMP20");
    }
    if (! aht.begin()) {
        Serial2.println("Could not find AHT? Check wiring");
    }
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL, /* Operating Mode. */
                    Adafruit_BMP280::SAMPLING_X2, /* Temp. oversampling */
                    Adafruit_BMP280::SAMPLING_X16, /* Pressure oversampling */
                    Adafruit_BMP280::FILTER_X16, /* Filtering. */
                    Adafruit_BMP280::STANDBY_MS_500);
    Serial2.println("Senser OK");
}

inline void Init_Wifi() {
    WiFiClass::mode(WIFI_STA);
    WiFi.begin(ssid, passwd);
    Serial2.printf("Connecting");
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(150);
        digitalWrite(ON_BOARD_LED,HIGH);
        delay(150);
        digitalWrite(ON_BOARD_LED,LOW);
    }
    Serial2.println("");
    Serial2.print("WiFi connected IP:");
    Serial2.print(WiFi.localIP());
    Serial2.println("");
}

inline void Init_Display() {
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(FONT);
    Serial2.println("U8G2 OK");
}

inline void Init_Time() {
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo{};

    time(&now);
    // 将时区设置为中国标准时间
    setenv("TZ", "CST-8", 1);
    tzset();

    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);  //时间的分辨率为 1S
    Serial2.println("Time OK");
}
#endif //UNTITLED_INIT_H