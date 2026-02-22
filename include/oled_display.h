//
// Created by Loogze on 2026/2/22.
//

#ifndef CLOCK_MORE_OLED_DISPLAY_H
#define CLOCK_MORE_OLED_DISPLAY_H

void oled_Display(tm time, const byte page,const unsigned long durin) {
    // Serial2.println("is going to rendar");
    u8g2.clearBuffer();
    if (!page) {
        aht.getEvent(&humidity, &temp);
        Temperature = bmp.readTemperature();
        Pressure = bmp.readPressure() / 1000.0;

        u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        u8g2.setCursor(0, 14);
        u8g2.printf("温度:%.1f*C", Temperature);
        u8g2.setCursor(0, 28);
        u8g2.printf("压强:%.1fkPa", Pressure);
        u8g2.setCursor(65, 14);
        u8g2.printf("湿度:%.1frH", humidity.relative_humidity);
    } else {
        u8g2.setFont(u8g2_font_fur20_tn);
        u8g2.setCursor(5, 32);
        u8g2.print(&time, "%T");
        u8g2.setCursor(10,9);
        u8g2.setFont(u8g2_font_wqy12_t_gb2312);
        u8g2.print(&time,"%F %a");
    }
    // u8g2.setFont(u8g2_font_wqy13_t_gb2312);
    // u8g2.drawStr(0,20,"hello");
    u8g2.setFont(u8g2_font_5x8_mf);
    u8g2.setCursor(115,26);
    u8g2.printf("%ld",durin);
    u8g2.setCursor(115,32);
    u8g2.printf("%.0f",1.0 / (durin / 1000.0));
    u8g2.sendBuffer();
}

#endif //CLOCK_MORE_OLED_DISPLAY_H