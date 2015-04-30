//
//  Beacon.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Beacon.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Beacon::Beacon(uint32_t robotId): AntzRobot(robotId), curFood(0xFF), curNest(0xFF), foodTimer(0), nestTimer(0) {
}

////////////////////////////////////////////////////////////////
// Setup
void Beacon::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void Beacon::loop() {
    motor.stop();
    uint16_t minFood = 0xFF; // to store the minimum food cardinality
    uint16_t minNest = 0xFF; // to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);
    display.green(false);
    bool wait = true; // a flag indicating whether there're more signals to be heard
    while (wait || minFood == 0xFF && minNest == 0xFF) {
        wait = false;
        unsigned long cur = millis();
        if (cur - nestTimer > 15000)
            curNest = 0xFF;
        if (cur - foodTimer > 15000)
            curFood = 0xFF;
        
        for (int i = 0; i < 6; ++i) {
            if (recver.canHearSignal(i)) {
                wait = true;
                uint32_t number; // to store the 32-bit signal
                if (recver.recvFrom(i, &number)) {
                    /*
                     uint16_t thisID = (number >> 16);
                     for (int i = 0; i < 5; ++i)
                     if (id[i][0] == thisID)
                     id[i][0] = 0;
                     for (int i = 0; i < 5; ++i) {
                     if (id[i][0] == 0) {
                     id[i][0] = thisID;
                     id[i][1] = millis();
                     break;
                     }
                     }
                     */
                    uint8_t nest = (uint8_t)(number & 0xFF);
                    uint8_t food = (uint8_t)(number >> 8);
                    if (nest > 0 && nest < minNest)
                        minNest = nest;
                    if (food > 0 && food < minFood)
                        minFood = food;
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
    delay(random(100) * 20);
    if (!recver.canHearSignal()) {
        uint32_t myNumber = 0;
        myNumber |= (identifier << 16);
        myNumber |= (curFood << 8);
        myNumber |= curNest;
        display.red(false);
        display.green(true);
        display.number(true, curFood);
        delay(200);
        display.number(true, curNest);
        sender.send(myNumber, 2000);
    }
}