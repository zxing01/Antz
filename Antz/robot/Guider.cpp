//
//  Guider.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Guider.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Guider::Guider(uint32_t robotId):
AntzRobot(robotId),
curFood(0xFF),
curNest(0xFF),
foodTimer(0),
nestTimer(0),
priority(DEFAULT_PRIORITY) {
}

////////////////////////////////////////////////////////////////
// Setup
void Guider::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void Guider::loop() {
    stopMoving();
    minFood = 0xFF; // to store the minimum food cardinality
    minNest = 0xFF; // to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);
    display.green(false);
    bool wait = true; // a flag indicating whether there're more signals to be heard
    while (wait || minNest == 0xFF && minFood == 0xFF)
        wait = receiveSignal();
    
    delay(random(priority) * 10);
    
    if (!recver.canHearSignal()) {
        priority = DEFAULT_PRIORITY;
        display.red(false);
        display.green(true);
        display.number(true, curFood);
        delay(100);
        display.number(true, curNest);
        sendSignal();
    }
    else if (priority >= 5)
        priority -= 5;
}

////////////////////////////////////////////////////////////////
// Receive signals from all the receivers
bool Guider::receiveSignal() {
    bool received = false;
    unsigned long cur = millis();
    if (cur - nestTimer > 10000)
        curNest = 0xFF;
    if (cur - foodTimer > 10000)
        curFood = 0xFF;
    
    for (int i = 0; i < 6; ++i) {
        if (recver.canHearSignal(i)) {
            received = true;
            uint32_t number; // to store the 32-bit signal
            if (recver.recvFrom(i, &number)) {
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
    return received;
}

////////////////////////////////////////////////////////////////
// Send signals from all the senders
void Guider::sendSignal() {
    uint32_t myNumber = 0;
    myNumber |= (identifier << 16);
    myNumber |= (curFood << 8);
    myNumber |= curNest;
    sender.send(myNumber, 500);
}