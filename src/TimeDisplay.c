//
// Created by loogze on 2026/2/19.
//

#include "TimeDisplay.h"

inline void seg_display(const int first, const int second,
                        const int third, const int forth )
{
    const int arr_num[4] = {first,second,third,forth};
    for (int seg = 0; seg < 4; ++seg) {
        for (int pos = 0; pos < 7; ++pos) {
            digitalWrite(pos_pins[pos], arr_seg[arr_num[seg]] [pos]);
        }
    }
}