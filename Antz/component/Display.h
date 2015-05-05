//
//  Display.h
//  Antz
//
//  Created by Zhi Xing on 2/19/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Display__
#define __Antz__Display__

#include <Arduino.h>

#define HIGH1       24
#define HIGH2       31
#define TOP         22
#define MID         29
#define BOT         26
#define TOPLEFT     23
#define TOPRGHT     30
#define BOTLEFT     25
#define BOTRGHT     28
#define DOT         27
#define RED         35
#define GREEN       34
#define BLUE        33
#define YELLOW      32

namespace Antz {
    class Display {
    public:
        Display();
        void red(bool on);
        void green(bool on);
        void blue(bool on);
        void yellow(bool on);
        void number(bool on, uint8_t n); // display n if n is in [1, 9], display 0 otherwise
        void number(bool on);
        uint8_t currentNumber();
    private:
        uint8_t curNum;
    };
}

#endif /* defined(__Antz__Display__) */
