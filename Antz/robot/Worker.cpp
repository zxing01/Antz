//
//  Worker.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Worker.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Worker::Worker(uint32_t robotId):
AntzRobot(robotId),
target(0),
curNumber(0xFFFFFFFF),
numberTimer(0),
minSignal(0xFF),
signalIndex(6),
minNumber(0xFFFFFFFF) {
}

////////////////////////////////////////////////////////////////
// Setup
void Worker::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void Worker::loop() {
    display.red(false);
    display.green(false);
    if (target == 0) { // i'm going towards nest
        display.blue(false);
        display.yellow(true);
    }
    else if (target == 1) {
        display.blue(true);
        display.yellow(false);
    }
    
    minSignal = 0xFF;
    signalIndex = 6;
    minNumber = 0xFFFFFFFF;
    bool received = receiveSignal();
    
    uint8_t cur = target == 0 ? curNumber : (curNumber >> 8);
    
    if (received && minSignal != 0xFF && (minSignal <= cur || millis() - numberTimer > 5000)) {
        display.number(true, min);
        curNumber = minNumber;
        numberTimer = millis();
        makeMovement();
    }
    else
        randomWalkGo();
    //else { // min > cur && millis() - numberTimer <= 3000
    //motor.forward();
    //}
}

////////////////////////////////////////////////////////////////
// Receive signal from all the recievers
bool Worker::receiveSignal() {
    bool received = false;
    int idx[6] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(idx[i], &number)) {
            received = true;
            randomWalkReset();
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            if (cardinality == 1)
                target = 1 - target;
            else if (cardinality > 0 && cardinality < minSignal) {
                minSignal = cardinality;
                signalIndex = idx[i];
                minNumber = number;
            }
        }
    }
    return received;
}


////////////////////////////////////////////////////////////////
// Make a movement basing on current signal index
void Worker::makeMovement() {
    switch (signalIndex) {
        case IDX_FRONT:
            if (avoid() == false)
                goForward(1000);
            break;
        case IDX_REAR:
            turnLeft(180);
            break;
        case IDX_LFRONT:
            turnLeft(60);
            break;
        case IDX_LREAR:
            turnLeft(120);
            break;
        case IDX_RFRONT:
            turnRight(60);
            break;
        case IDX_RREAR:
            turnRight(120);
            break;
    }
}

