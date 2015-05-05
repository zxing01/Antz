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
#include "Tester.h"
#include "BayesWorker.h"

using namespace Antz;

uint32_t AntzRobot::identifier = 0;
uint16_t AntzRobot::movePhase = 0;
float AntzRobot::likelihood[] = {1.f/6};
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
    while (scanner.scan(&angle) <= 40) {
        detected = true;
        if (angle > 90)
            turnRight(60);
        else
            turnLeft(60);
    }
    goForward(1500);
    return detected;
}

////////////////////////////////////////////////////////////////
// Update likelihoods using Bayes rule according to new signals
void AntzRobot::bayesUpdate(bool signals[]) {
    if (sizeof(signals)/sizeof(bool) != 6)
        return;
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i) {
        float condProb, evidProb;
        if (signals[i]) {
            condProb = 9.f / 10;
            evidProb = 1.f / 6;
        }
        else {
            condProb = 1.f / 10;
            evidProb = 5.f / 6;
        }
        likelihood[i] = condProb * likelihood[i] / evidProb;
    }
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Update likelihoods using Bayes rule according to new movement
void AntzRobot::bayesUpdate() {
    int64_t duration = 0;
    Timer3.detachInterrupt();
    if (motorStartMillis > 0) {
        duration = millis() - motorStartMillis;
        motorStartMillis = millis();
    }
    
    if (curMovement == mt_forward || curMovement == mt_backward) {
        float avg = (1 - likelihood[IDX_FRONT] - likelihood[IDX_REAR]) / 4;
        likelihood[IDX_LFRONT] = avg;
        likelihood[IDX_LREAR] = avg;
        likelihood[IDX_RFRONT] = avg;
        likelihood[IDX_RREAR] = avg;
    }
    else {
        int64_t shifts = duration / MTR_MSPERDEG / 60; // TODO: consider 60 for (30, 90) instead of (60, 119)
        shifts = shifts % 6;
        float temp[shifts];
        if (curMovement == mt_left) { // array shifts right
            for (int i = shifts; i < 6 + shifts; ++i) {
                if (i < 6) {
                    temp[i - shifts] = likelihood[i];
                    likelihood[i] = likelihood[i - shifts];
                }
                else
                    likelihood[i - 6] = temp[i - 6];
            }
        }
        else if (curMovement == mt_right) { // array shifts left
            for (int i = 0; i < 6; ++i) {
                if (i + shifts < 6) {
                    temp[i] = likelihood[i];
                    likelihood[i] = likelihood[i + shifts];
                }
                else
                    likelihood[i] = temp[i - shifts];
            }
        }
    }
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Reset likelihood
void AntzRobot::bayesReset() {
    Timer3.detachInterrupt();
    for (int i = 0; i < 6; ++i)
        likelihood[i] = 1.f / 6.f;
    Timer3.attachInterrupt(isr);
}

////////////////////////////////////////////////////////////////
// Get the desicion from the Bayesian model
uint8_t AntzRobot::bayesDecision() {
    bayesUpdate();
    Timer3.detachInterrupt();
    int idx[] = {IDX_FRONT, IDX_LFRONT, IDX_RFRONT, IDX_LREAR, IDX_RREAR, IDX_REAR};
    for (int i = 0; i < 6; ++i) {
        if (likelihood[idx[i]] > 0.8) {
            Timer3.attachInterrupt(isr);
            return idx[i];
        }
    }
    Timer3.attachInterrupt(isr);
    return IDX_NULL;
}

////////////////////////////////////////////////////////////////
// Reset random walk state
void AntzRobot::randomWalkReset() {
    movePhase = 0;
}

////////////////////////////////////////////////////////////////
// Make one random movement
void AntzRobot::randomWalkGo() {
    switch (movePhase) {
        case 0:
            ++movePhase;
            break;
        case 1:
            ++movePhase;
            break;
        case 2:
            ++movePhase;
            break;
        default:
            ++movePhase;
            break;
    }
}

////////////////////////////////////////////////////////////////
// The ISR for timer interrupts, this should be called in
// all the subclasses' ISRs
void AntzRobot::isr() {
    if (motorStopMillis >= 0 && millis() >= motorStopMillis)
        stopMoving();
}
