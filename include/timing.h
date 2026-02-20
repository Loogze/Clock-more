//
// Created by wppuq on 2026/2/20.
//

#ifndef CLOCK_MORE_TIMING_H
#define CLOCK_MORE_TIMING_H

#define NTP1  "ntp1.aliyun.com"
#define NTP2  "ntp2.aliyun.com"
#define NTP3  "ntp3.aliyun.com"

inline void Init_Time() {
    time_t now;
    char strftime_buf[64];
    struct tm timeinfo{};
    time(&now);
    setenv("TZ", "CST-8", 1);// 将时区设置为中国标准时间
    tzset();
    localtime_r(&now, &timeinfo);
    strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
    ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);  //时间的分辨率为 1S
    Serial2.println("Time OK");
}

inline tm updateClock() {
    struct tm timeinfo{};
    getLocalTime(&timeinfo);
    Serial2.println("Fetching Time");
    configTime(8 * 3600, 0, NTP1, NTP2,NTP3);
    Serial2.println(&timeinfo, "Time is: %F %T %A");
    return timeinfo;
}
#endif //CLOCK_MORE_TIMING_H