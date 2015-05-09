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
minNumber(0xFFFFFFFF),
movePhase(0),
noMoveCnt(0),
randomMoveTimer(0){
}

////////////////////////////////////////////////////////////////
// Setup
void Worker::setup() {
    AntzRobot::setup();
    Serial.begin(9600);
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
    receiveSignal();
    
    uint8_t cur = target == 0 ? curNumber : (curNumber >> 8);
    
    if (minSignal != 0xFF && minSignal <= cur) {
        display.number(true, minSignal);
        curNumber = minNumber;
        numberTimer = millis();
        makeMovement();
        randomWalkReset();
        noMoveCnt = 0;
    }
    else if (curNumber == 0xFFFFFFFF) {
        if (millis() - randomMoveTimer > 1000) {
            randomWalkGo();
            randomMoveTimer = millis();
        }
    }
}

////////////////////////////////////////////////////////////////
// Receive signal from all the recievers
bool Worker::receiveSignal() {
    if (millis() - numberTimer > 5000) {
        curNumber = 0xFFFFFFFF;
        numberTimer = millis();
    }
    bool received = false;
    int idx[6] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(idx[i], &number)) {
            received = true;
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            if (cardinality == 1) {
                target = 1 - target;
                received = false;
                break;
            }
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
                goForward(500);
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

////////////////////////////////////////////////////////////////
// Reset random walk state
void Worker::randomWalkReset() {
    movePhase = 0;
}

////////////////////////////////////////////////////////////////
// Make one random movement
void Worker::randomWalkGo() {
    switch (movePhase) {
        case 0:
            turnLeft(30);
            break;
        case 1:
            turnRight(30);
            break;
        case 2:
            turnRight(60, false);
            if (!avoid())
                goForward(500);
            break;
        default:
            turnLeft(60, false);
            int cnt = movePhase;
            while (cnt-- > 0) {
                if (!avoid())
                    goForward(500);
            }
            break;
    }
    ++movePhase;
}

