//
//  Display.cpp
//  Antz
//
//  Created by Zhi Xing on 2/19/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Display.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
Display::Display():curNum(0) {
    pinMode(HIGH1, OUTPUT);
    pinMode(HIGH2, OUTPUT);
    pinMode(TOP, OUTPUT);
    pinMode(MID, OUTPUT);
    pinMode(BOT, OUTPUT);
    pinMode(TOPLEFT, OUTPUT);
    pinMode(TOPRGHT, OUTPUT);
    pinMode(BOTLEFT, OUTPUT);
    pinMode(BOTRGHT, OUTPUT);
    pinMode(DOT, OUTPUT);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    pinMode(YELLOW, OUTPUT);
}

////////////////////////////////////////////////////////////////
void Display::red(bool on) {
    if (on)
        digitalWrite(RED, HIGH);
    else
        digitalWrite(RED, LOW);
}

////////////////////////////////////////////////////////////////
void Display::green(bool on) {
    if (on)
        digitalWrite(GREEN, HIGH);
    else
        digitalWrite(GREEN, LOW);
}

////////////////////////////////////////////////////////////////
void Display::blue(bool on) {
    if (on)
        digitalWrite(BLUE, HIGH);
    else
        digitalWrite(BLUE, LOW);
}

////////////////////////////////////////////////////////////////
void Display::yellow(bool on) {
    if (on)
        digitalWrite(YELLOW, HIGH);
    else
        digitalWrite(YELLOW, LOW);
}

////////////////////////////////////////////////////////////////
void Display::number(bool on, uint8_t n) {
    curNum = n;
    if (!on) {
        digitalWrite(HIGH1, LOW);
        digitalWrite(HIGH2, LOW);
    }
    else {
        digitalWrite(HIGH1, HIGH);
        digitalWrite(HIGH2, HIGH);
        switch (n) {
            case 0:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, HIGH);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, LOW);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 1:
                digitalWrite(TOP, HIGH);
                digitalWrite(MID, HIGH);
                digitalWrite(BOT, HIGH);
                digitalWrite(TOPLEFT, HIGH);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 2:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, HIGH);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, LOW);
                digitalWrite(BOTRGHT, HIGH);
                digitalWrite(DOT, HIGH);
                break;
            case 3:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, HIGH);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 4:
                digitalWrite(TOP, HIGH);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, HIGH);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 5:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, HIGH);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 6:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, HIGH);
                digitalWrite(BOTLEFT, LOW);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 7:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, HIGH);
                digitalWrite(BOT, HIGH);
                digitalWrite(TOPLEFT, HIGH);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 8:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, LOW);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            case 9:
                digitalWrite(TOP, LOW);
                digitalWrite(MID, LOW);
                digitalWrite(BOT, LOW);
                digitalWrite(TOPLEFT, LOW);
                digitalWrite(TOPRGHT, LOW);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, LOW);
                digitalWrite(DOT, HIGH);
                break;
            default:
                digitalWrite(TOP, HIGH);
                digitalWrite(MID, HIGH);
                digitalWrite(BOT, HIGH);
                digitalWrite(TOPLEFT, HIGH);
                digitalWrite(TOPRGHT, HIGH);
                digitalWrite(BOTLEFT, HIGH);
                digitalWrite(BOTRGHT, HIGH);
                digitalWrite(DOT, LOW);
                break;
        }
    }
}

////////////////////////////////////////////////////////////////
void Display::number(bool on) {
    number(on, curNum);
}

////////////////////////////////////////////////////////////////
uint8_t Display::currentNumber() {
    return curNum;
}
