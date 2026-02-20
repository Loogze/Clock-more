//
// Created by wppuq on 2026/2/19.
//

#ifndef UNTITLED_TIMEDISPLAY_H
#define UNTITLED_TIMEDISPLAY_H
#include <Arduino.h>

const byte arr_seg[10][7] = {
    {1, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0, 0, 0},
    {1, 1, 0, 1, 0, 1, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {0, 1, 1, 0, 0, 1, 1},
    {1, 0, 1, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1}
};

const int seg_pins[4] = {39, 37, 38, 13};
const int pos_pins[7] = {14, 3, 7, 42, 5, 2, 45};
const int dt_pin = 4;

inline void seg_display(int first, int second,int third, int forth ,byte dp);

#endif //UNTITLED_TIMEDISPLAY_H
