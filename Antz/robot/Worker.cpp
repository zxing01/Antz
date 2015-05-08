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
movePhase(0),
noMoveCnt(0) {
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
        
        //display.number(true, nestDirect);
    }
    else if (target == 1) {
        display.blue(true);
        display.yellow(false);
        
        //display.number(true, foodDirect);
    }
    
    receiveSignal();
    
    if (target == 0 && minNest == 1) {
        target = 1;
        minFood = curMinFood;
        foodTimer = millis();
    }
    else if (target == 1 && minFood == 1) {
        target = 0;
        minNest = curMinNest;
        nestTimer = millis();
    }
    
    //uint8_t idx[6] = {IDX_FRONT, IDX_RFRONT, IDX_RREAR, IDX_REAR, IDX_LREAR, IDX_LFRONT};
    //uint8_t idx_cur, idx_est;
    
    if (target == 0 && curMinNest != 0xFF && curMinNest <= minNest) { // go to nest
        Serial.println("1");
        //for (int i = 0; i < 6; ++i) {
            //if (nestDirect == idx[i])
                //idx_est = i;
            //else if (nestIndex == idx[i])
                //idx_cur = i;
        //}
        display.number(true, curMinNest);
        //if ((idx_est - idx_cur) % 6 == 1 || (idx_est - idx_cur) % 6 == 5)
            makeMovement(nestIndex);
        randomWalkReset();
        noMoveCnt = 0;
    }
    else if (target == 1 && curMinFood != 0xFF && curMinFood <= minFood) { // go to food
        Serial.println("2");
        //for (int i = 0; i < 6; ++i) {
            //if (foodDirect == idx[i])
                //idx_est = i;
            //else if (foodIndex == idx[i])
                //idx_cur = i;
        //}
        display.number(true, curMinFood);
        //if ((idx_est - idx_cur) % 6 == 1 || (idx_est - idx_cur) % 6 == 5)
            makeMovement(foodIndex);
        randomWalkReset();
        noMoveCnt = 0;
    }
    /*else {
        Serial.println("3");
        bayesUpdate();
        if (target == 0 && nestDirect != IDX_NULL && minNest == 0xFF) {
            //display.number(true, 0xFF);
            makeMovement(nestDirect);
        }
        else if (target == 1 && foodDirect != IDX_NULL && minFood == 0xFF) {
            //display.number(true, 0xFF);
            makeMovement(foodDirect);
        }
        //else if (noMoveCnt >= 10) {
            //display.number(true, 0xFF);
            //randomWalkGo();
        //}
        ++noMoveCnt;
    }
     */
}

////////////////////////////////////////////////////////////////
// Make a movement basing on current signal index
void Worker::makeMovement(uint8_t index) {
    switch (index) {
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
                goForward(100 * movePhase, false);
            break;
        default:
            turnLeft(60, false);
            if (!avoid())
                goForward(100 * movePhase, false);
            break;
    }
    ++movePhase;
}

