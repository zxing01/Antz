//
//  AntzRobot.cpp
//  AntzRobot
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "AntzRobot.h"
#include "Beacon.h"
#include "Walker.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Constructor
AntzRobot::AntzRobot(uint32_t robotId): identifier(robotId), movePhase(0) {
}

////////////////////////////////////////////////////////////////
// Object factory
AntzRobot* AntzRobot::createAntzRobot(char* type, uint32_t robotId) {
    if (strcmp(type, "Walker") == 0)
        return new Walker(robotId);
    else if (strcmp(type, "Beacon") == 0)
        return new Beacon(robotId);
    else
        return NULL;
}

////////////////////////////////////////////////////////////////
// Setup
void AntzRobot::setup() {
    scanner.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
// Main loop
void AntzRobot::loop() {
    
}

////////////////////////////////////////////////////////////////
// Obstacle avoidance
bool AntzRobot::avoid() {
    bool detected = false;
    double angle;
    if (scanner.scan(&angle) <= 17) {
        detected = true;
        motor.backward(500);
        motor.turnLeft(500);
        while (scanner.scan(&angle) <= 17)
            motor.turnLeft();
        motor.forward(1000);
    }
    return detected;
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
            motor.turnLeft(random(60));
            ++movePhase;
            break;
        case 1:
            motor.turnRight(random(60));
            ++movePhase;
            break;
        case 2:
            motor.turnRight(random(60));
            motor.forward(random(500, 1000));
            ++movePhase;
            break;
        default:
            motor.turnLeft(random(90, 150));
            motor.forward(random(500, 1000));
            break;
    }
}
