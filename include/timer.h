//
// Created by Loogze on 2026/2/22.
//

#ifndef CLOCK_MORE_TIMER_H
#define CLOCK_MORE_TIMER_H
#include <Ticker.h>
tm Timeinfo;


const unsigned long Time_To_UpDate = 1000 * 60 * 10;

void timerForNTP() {
    if (!getLocalTime(&Timeinfo)) {
        Serial2.println("NTP sync failed, retrying...");
        // 可选：添加重试逻辑，但避免频繁 configTime
        configTime(8 * 3600,
               0,
               NTP1,
               NTP2,
               NTP3);
    } else {
        Serial2.println("Time updated via NTP");
    }
}

void timerUpdateTime() {
    if (!getLocalTime(&Timeinfo)) {
        updateClock();
    }
}


// Ticker tickerSenser;
Ticker tickerNTP;
Ticker tickerUpdateTime;

volatile byte switcher = 1;
volatile byte switcher_statis = 0;
volatile byte pressed = 0;

hw_timer_t *SenserTimer = NULL;

inline void touchInterrupt() {
    if (touchRead(1) > 150000 && switcher_statis == 0) {
        switcher = !switcher;
        switcher_statis = 1;
        pressed =1 ;
    }else {
        switcher_statis = 0 ;
    }
}

inline void Init_Timer() {

    // SenserTimer = timerBegin(0,80,true);
    // timerAttachInterrupt(SenserTimer,timerForSenser,true);
    // timerAlarmWrite(SenserTimer,0.5*1000000,true);
    // timerAlarmEnable(SenserTimer);
    touchAttachInterrupt(1,touchInterrupt,120000);

    tickerNTP.attach(Time_To_UpDate,timerForNTP);
    // tickerSenser.attach_ms(100,timerForSenser);
    tickerUpdateTime.attach_ms(10,timerUpdateTime);
}


#endif //CLOCK_MORE_TIMER_H