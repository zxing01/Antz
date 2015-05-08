//
//  AntzRobot.cpp
//  AntzRobot
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "AntzRobot.h"
#include "Worker.h"
#include "Guider.h"
#include "ExpGuider.h"
#include "Tester.h"
#include "BayesWorker.h"

using namespace Antz;

uint8_t AntzRobot::curMinFood = 0xFF;
uint8_t AntzRobot::curMinNest = 0xFF;
uint64_t AntzRobot::foodIndex = IDX_NULL;
uint64_t AntzRobot::nestIndex = IDX_NULL;
uint8_t AntzRobot::minFood = 0xFF;
uint8_t AntzRobot::minNest = 0xFF;
uint64_t AntzRobot::foodTimer = 0;
uint64_t AntzRobot::nestTimer = 0;
uint8_t AntzRobot::trueMinFood = 0xFF;
uint8_t AntzRobot::trueMinNest = 0xFF;
uint8_t AntzRobot::foodDirect = IDX_NULL;
uint8_t AntzRobot::nestDirect = IDX_NULL;

uint8_t AntzRobot::avoidCnt = 0;
uint32_t AntzRobot::identifier = 0;
float AntzRobot::condProb[] = {1.f/6};
int64_t AntzRobot::motorStartMillis = -1;
int64_t AntzRobot::motorStopMillis = -1;

AntzRobot::MoveType AntzRobot::curMovement = mt_stop;
Display AntzRobot::display;
Motor AntzRobot::motor;
Receiver AntzRobot::recver;
Scanner AntzRobot::scanner;

////////////////////////////////////////////////////////////////
// Constructor
AntzRobot::AntzRobot(uint32_t robotId) {
    identifier = robotId;
}

////////////////////////////////////////////////////////////////
// Object factory
AntzRobot* AntzRobot::createAntzRobot(char* type, uint32_t robotId) {
    if (strcmp(type, "Worker") == 0)
        return new Worker(robotId);
    else if (strcmp(type, "Guider") == 0)
        return new Guider(robotId);
    else if (strcmp(type, "ExpGuider") == 0)
        return new ExpGuider(robotId);
    else if (strcmp(type, "Tester") == 0)
        return new Tester(robotId);
    else if (strcmp(type, "BayesWorker") == 0)
        return new BayesWorker(robotId);
    else
        return NULL;
}

////////////////////////////////////////////////////////////////
// Setup
void AntzRobot::setup() {
    Timer3.initialize(100000); // run the ISR every 100 ms
    Timer3.attachInterrupt(isr);
    scanner.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
// Main loop
void AntzRobot::loop() {
}

////////////////////////////////////////////////////////////////
// Asynchronously go forward for msecs
void AntzRobot::goForward(uint64_t msecs, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();
    motor.forward();
    curMovement = mt_forward;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + msecs;
    Timer3.attachInterrupt(isr);
    if (!async) {
        delay(msecs);
        stopMoving();
    }
}

////////////////////////////////////////////////////////////////
// Asynchronously go backward for msecs
void AntzRobot::goBackward(uint64_t msecs, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();
    motor.backward();
    curMovement = mt_backward;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + msecs;
    Timer3.attachInterrupt(isr);
    if (!async) {
        delay(msecs);
        stopMoving();
    }
}

////////////////////////////////////////////////////////////////
// Asynchronously turn left for degree
void AntzRobot::turnLeft(float degree, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();
    motor.left();
    curMovement = mt_left;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + MTR_MSPERDEG * degree;
    Timer3.attachInterrupt(isr);
    if (!async) {
        delay(MTR_MSPERDEG * degree);
        stopMoving();
    }
}

////////////////////////////////////////////////////////////////
// Asynchronously turn right for degree
void AntzRobot::turnRight(float degree, bool async) {
    bayesUpdate();
    Timer3.detachInterrupt();
    motor.right();
    curMovement = mt_right;
    motorStartMillis = millis();
    motorStopMillis = motorStartMillis + MTR_MSPERDEG * degree;
    Timer3.attachInterrupt(isr);
    if (!async) {
        delay(MTR_MSPERDEG * degree);
        stopMoving();
    }
}

////////////////////////////////////////////////////////////////
// Immediately stop moving
void AntzRobot::stopMoving() {
    bayesUpdate();
    Timer3.detachInterrupt();
    motor.stop();
    curMovement = mt_stop;
    motorStartMillis = -1;
    motorStopMillis = -1;
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Obstacle avoidance
bool AntzRobot::avoid() {
    bool detected = false;
    float angle;
    bool async = true;
    uint8_t deg = 60;
    while (scanner.scan(&angle) <= 35) {
        detected = true;
        ++avoidCnt;
        if (avoidCnt > 5) { // possible deadlock
            async = false;
            deg = 90;
        }
        //if (angle > 90)
            //turnRight(deg, async);
        //else
            turnLeft(deg, async);
    }
    goForward(1000);
    if (!detected)
        avoidCnt = 0;
    return detected;
}

////////////////////////////////////////////////////////////////
// Update conditional probabilities using Bayes rule according
//   to new signals
void AntzRobot::bayesUpdate(bool signals[]) {
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i) {
        if (signals[i]) {
            float marginal = PROB_TPOS * condProb[i] + PROB_FPOS * (1 - condProb[i]);
            condProb[i] = PROB_TPOS * condProb[i] / marginal;
        }
        else {
            float marginal = PROB_TNEG * (1 - condProb[i]) + PROB_FNEG * condProb[i];
            condProb[i] = 1 - PROB_TNEG * (1 - condProb[i]) / marginal;
        }
    }
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Update conditional probabilities using Bayes rule according
//   to new movement
void AntzRobot::bayesUpdate() {
    int64_t duration = 0;
    Timer3.detachInterrupt();
    if (motorStartMillis > 0) {
        duration = millis() - motorStartMillis;
        motorStartMillis = millis();
    }
    if (curMovement == mt_forward || curMovement == mt_backward) {
        float avg = (1 - condProb[IDX_FRONT] - condProb[IDX_REAR]) / 4;
        condProb[IDX_LFRONT] = avg;
        condProb[IDX_LREAR] = avg;
        condProb[IDX_RFRONT] = avg;
        condProb[IDX_RREAR] = avg;
    }
    else if (curMovement == mt_left || curMovement == mt_right) {
        int shifts = (duration / MTR_MSPERDEG - 30) / 60 + 1;
        shifts = shifts % 6;
        float temp[shifts];
        if (curMovement == mt_left) { // array shifts right
            for (int i = 5; i >= 0; --i) {
                if (i + shifts >= 6)
                    temp[i + shifts - 6] = condProb[i];
                if (i - shifts >= 0)
                    condProb[i] = condProb[i - shifts];
                else
                    condProb[i] = temp[i];
            }
        }
        else if (curMovement == mt_right) { // array shifts left
            for (int i = 0; i < 6; ++i) {
                if (i - shifts < 0)
                    temp[i] = condProb[i];
                if (i + shifts < 6)
                    condProb[i] = condProb[i + shifts];
                else
                    condProb[i] = temp[i + shifts - 6];
            }
        }
    }
    
    directUpdate(curMovement, duration); // update target directions
    
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Reset conditional probabilities
void AntzRobot::bayesReset() {
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i)
        condProb[i] = 1.f / 6;
    curMovement = mt_unknown;
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Get the desicion from the Bayesian model
uint8_t AntzRobot::bayesDecision() {
    bayesUpdate();
    Timer3.detachInterrupt();
    int idx[] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) {
        if (condProb[idx[i]] > 0.8) {
            Timer3.attachInterrupt(isr);
            return idx[i];
        }
    }
    Timer3.attachInterrupt(isr);
    return IDX_NULL;
}

////////////////////////////////////////////////////////////////
// The ISR for timer interrupts, this should be called in
// all the subclasses' ISRs
void AntzRobot::isr() {
    if (motorStopMillis >= 0 && millis() >= motorStopMillis)
        stopMoving();
}

////////////////////////////////////////////////////////////////
// Receive signals from all the receivers
bool AntzRobot::receiveSignal() {
    bool received = false;
    unsigned long cur = millis();
    if (cur - nestTimer > SIG_PRSV)
        minNest = 0xFF;
    if (cur - foodTimer > SIG_PRSV)
        minFood = 0xFF;
    
    curMinFood = curMinNest = 0xFF;
    foodIndex = nestIndex = IDX_NULL;
    uint8_t idx[6] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) {
        if (recver.canHearSignal(i)) {
            uint32_t number; // to store the 32-bit signal
            if (recver.recvFrom(idx[i], &number)) {
                received = true;
                uint8_t nest = (uint8_t)(number & 0xFF);
                uint8_t food = (uint8_t)(number >> 8);
                if (nest > 0 && nest < minNest) {
                    curMinNest = nest;
                    nestIndex = idx[i];
                }
                if (food > 0 && food < minFood) {
                    curMinFood = food;
                    foodIndex = idx[i];
                }
            }
        }
    }
    if (curMinNest <= minNest) {
        minNest = curMinNest;
        nestTimer = millis();
    }
    if (curMinFood <= minFood) {
        minFood = curMinFood;
        foodTimer = millis();
    }
    if (curMinNest <= trueMinNest) {
        trueMinNest = curMinNest;
        nestDirect = nestIndex;
    }
    if (curMinFood <= trueMinFood) {
        trueMinFood = curMinFood;
        foodDirect = foodIndex;
    }
    return received;
}

////////////////////////////////////////////////////////////////
// Update estimated food and nest directions according to new
//   movement (only called in bayesUpdate method)
void AntzRobot::directUpdate(MoveType moveType, uint64_t moveDuration) {
    uint8_t idx[6] = {IDX_FRONT, IDX_RFRONT, IDX_RREAR, IDX_REAR, IDX_LREAR, IDX_LFRONT};
    int shifts = ((moveDuration / MTR_MSPERDEG - 30) / 60 + 1) % 6;
    int index;

    if (moveType == mt_forward) {
        switch (nestDirect) {
            case IDX_FRONT: break;
            case IDX_LFRONT: nestDirect = IDX_LREAR; break;
            case IDX_RFRONT: nestDirect = IDX_RREAR; break;
            default: nestDirect = IDX_REAR; break;
        }
        
        switch (foodDirect) {
            case IDX_FRONT: break;
            case IDX_LFRONT: foodDirect = IDX_LREAR; break;
            case IDX_RFRONT: foodDirect = IDX_RREAR; break;
            default: foodDirect = IDX_REAR; break;
        }
    }
    else if (moveType == mt_backward) {
        switch (nestDirect) {
            case IDX_REAR: break;
            case IDX_LREAR: nestDirect = IDX_LFRONT; break;
            case IDX_RREAR: nestDirect = IDX_RFRONT; break;
            default: nestDirect = IDX_FRONT; break;
        }
        
        switch (foodDirect) {
            case IDX_REAR: break;
            case IDX_LREAR: foodDirect = IDX_LFRONT; break;
            case IDX_RREAR: foodDirect = IDX_RFRONT; break;
            default: foodDirect = IDX_FRONT; break;
        }
    }
    else if (moveType == mt_left) {
        switch (nestDirect) {
            case IDX_FRONT: index = 0; break;
            case IDX_RFRONT: index = 1; break;
            case IDX_RREAR: index = 2; break;
            case IDX_REAR: index = 3; break;
            case IDX_LREAR: index = 4; break;
            case IDX_LFRONT: index = 5; break;
        }
        nestDirect = idx[(index + shifts) % 6];
        switch (foodDirect) {
            case IDX_FRONT: index = 0; break;
            case IDX_RFRONT: index = 1; break;
            case IDX_RREAR: index = 2; break;
            case IDX_REAR: index = 3; break;
            case IDX_LREAR: index = 4; break;
            case IDX_LFRONT: index = 5; break;
        }
        foodDirect = idx[(index + shifts) % 6];
    }
    else if (moveType == mt_right) {
        switch (nestDirect) {
            case IDX_FRONT: index = 0; break;
            case IDX_RFRONT: index = 1; break;
            case IDX_RREAR: index = 2; break;
            case IDX_REAR: index = 3; break;
            case IDX_LREAR: index = 4; break;
            case IDX_LFRONT: index = 5; break;
        }
        nestDirect = idx[(index - shifts) % 6];
        switch (foodDirect) {
            case IDX_FRONT: index = 0; break;
            case IDX_RFRONT: index = 1; break;
            case IDX_RREAR: index = 2; break;
            case IDX_REAR: index = 3; break;
            case IDX_LREAR: index = 4; break;
            case IDX_LFRONT: index = 5; break;
        }
        foodDirect = idx[(index - shifts) % 6];
    }
}


