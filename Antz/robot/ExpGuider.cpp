//
//  ExpGuider.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "ExpGuider.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
ExpGuider::ExpGuider(uint32_t robotId): AntzRobot(robotId), curFood(0xFF), curNest(0xFF), foodTimer(0), nestTimer(0) {
}

////////////////////////////////////////////////////////////////
// Setup
void ExpGuider::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void ExpGuider::loop() {
    if (cnt > 3)
        explore();
    else
        signal();
}

////////////////////////////////////////////////////////////////
// Receive and send signals
void ExpGuider::signal() {
    stopMoving();
    uint16_t minFood = 0xFF; // to store the minimum food cardinality
    uint16_t minNest = 0xFF; // to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);
    display.green(false);
    bool wait = true; // a flag indicating whether there're more signals to be heard
    bool gotFood = false;
    bool gotNest = false;
    
    while (wait || minFood == 0xFF && minNest == 0xFF) {
        wait = false;
        unsigned long cur = millis();
        if (cur - nestTimer > 5000)
            curNest = 0xFF;
        if (cur - foodTimer > 5000)
            curFood = 0xFF;
        
        for (int i = 0; i < 6; ++i) {
            if (recver.canHearSignal(i)) {
                wait = true;
                uint32_t number; // to store the 32-bit signal
                if (recver.recvFrom(i, &number)) {
                    uint8_t nest = (uint8_t)(number & 0xFF);
                    uint8_t food = (uint8_t)(number >> 8);
                    if (nest > 0 && nest < minNest) {
                        gotNest = true;
                        minNest = nest;
                    }
                    if (food > 0 && food < minFood) {
                        gotFood = true;
                        minFood = food;
                    }
                }
            }
        }
        if (minNest < (uint16_t)0xFF && minNest + 1 <= curNest) {
            curNest = minNest + 1;
            nestTimer = millis();
        }
        if (minFood < (uint16_t)0xFF && minFood + 1 <= curFood) {
            curFood = minFood + 1;
            foodTimer = millis();
        }
    }
    delay(random(50) * 10);
    if (!recver.canHearSignal()) {
        if (!gotFood || !gotNest)
            ++cnt;
        else
            cnt = 0;
        uint32_t myNumber = 0;
        myNumber |= (identifier << 16);
        myNumber |= (curFood << 8);
        myNumber |= curNest;
        display.red(false);
        display.green(true);
        display.number(true, curFood);
        delay(200);
        display.number(true, curNest);
        sender.send(myNumber, 500);
    }
}

////////////////////////////////////////////////////////////////
// Explore around
void ExpGuider::explore() {
    int idx[6] = {IDX_FRONT, IDX_RFRONT, IDX_RREAR, IDX_REAR, IDX_LREAR, IDX_LFRONT};
    int i;
    for (i = 0; i < 6; ++i) { // poll from 6 receivers
        if (recver.recvFrom(idx[i], NULL))
            break;
    }

    if (i < 6) {
        int index = idx[(i + 3) % 6];
        switch (index) {
            case IDX_FRONT:
                turnLeft(180);
                break;
            case IDX_REAR:
                if (avoid() == false)
                    goForward(1500);
                break;
            case IDX_LFRONT:
                turnRight(120);
                break;
            case IDX_LREAR:
                turnRight(60);
                break;
            case IDX_RFRONT:
                turnLeft(120);
                break;
            case IDX_RREAR:
                turnLeft(60);
                break;
        }
    }
    else
        goBackward(500);
}
