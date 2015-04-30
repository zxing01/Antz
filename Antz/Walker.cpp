//
//  Walker.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Walker.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Walker::Walker(uint32_t robotId): AntzRobot(robotId), target(0), curSource(0xFFFFFFFF), sourceTime(0) {
}

////////////////////////////////////////////////////////////////
// Setup
void Walker::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void Walker::loop() {
    display.red(false);
    display.green(false);
    motor.stop();
    if (target == 0) { // i'm going towards nest
        display.blue(false);
        display.yellow(true);
    }
    else if (target == 1) {
        display.blue(true);
        display.yellow(false);
    }
    
    // signal format (high to low): 16-bit ID + 8-bit food cardinality + 8-bit nest cardinality
    uint8_t min = 0xFF; // cardinality value I'm following
    uint8_t index = 6; // index of the receiver that receives the signal
    uint32_t minNumber = 0xFFFFFFFF; // 32-bit signal containing the min value
    bool received = false;
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(i, &number)) {
            received = true;
            randomWalkReset();
            uint8_t cardinality = target == 0 ? number : (number >> 8);
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
            
            if (cardinality == 1)
                target = 1 - target;
            else if (cardinality > 0 && cardinality < min) {
                min = cardinality;
                index = i;
                minNumber = number;
            }
        }
    }
    
    uint8_t cur = target == 0 ? curSource : (curSource >> 8);
    
    if (received && min != 0xFF && (min <= cur || millis() - sourceTime > 10000)) {
        display.number(true, min);
        curSource = minNumber;
        sourceTime = millis();
        if (index == 0) {
            int cnt = 0;
            while (avoid() == false && cnt < 3) {
                motor.forward(500);
                ++cnt;
            }
        }
        else if (index < 3)
            motor.turnRight(index * 60);
        else
            motor.turnLeft((6 - index) * 60);
        if (!avoid()) {
            motor.forward(1000);
        }
    }
    else {
        randomWalkGo();
    }
    //else { // min > cur && millis() - sourceTime <= 3000
    //motor.forward();
    //}
}