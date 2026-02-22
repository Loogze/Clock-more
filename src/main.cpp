#include <Arduino.h>
#include <TimeLib.h>

#include "Nixie.h"
#include "timing.h"
#include "Init.h"
#include "oled_display.h"
#include "timer.h"



void setup() {
    const unsigned long start = millis();
    Init_System();
    Init_Display();
    Init_Senser();
    Init_Wifi();
    Timeinfo = Init_Time();
    Init_Timer();

    Serial2.printf("ALL thing OK,taking %lu MS", millis() - start);
}

static unsigned long mill;
static unsigned long end;

void loop() {
    mill = millis();

    oled_Display(Timeinfo, switcher, end); //渲染屏幕
    digitalWrite(ON_BOARD_LED,!digitalRead(ON_BOARD_LED));
    end = millis() - mill;
}



