#include "utils.h"

#include <iostream>
#include <ncurses.h>

namespace Driving_Sim {
    double utils::kw_to_hp(const double kw) {
        return CONVERSION_FACTOR_KW_TO_HP * kw;
    }

    double utils::hp_to_kw(const double hp) {
        return hp / CONVERSION_FACTOR_KW_TO_HP;
    }

     /**
     * -> 261
     * <- 260
     * v| 258
     * ^| 259
     *
     * z 122
     * q 113
     * s 115
     * d 100
     *
     * w 119
     * a 97
     * s 115
     * d 100
     */
    void utils::registerKeys() {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);

        int ch;
        while ((ch = getch()) != 27) {
            switch (ch) {
                case 259:
                case 122: printf("↑\n"); break;
                case 260:
                case 113: printf("←\n"); break;
                case 258:
                case 115: printf("↓\n"); break;
                case 261:
                case 100: printf("→\n"); break;
                default: ;
            }
            refresh();
        }

        endwin();
    }
} // std