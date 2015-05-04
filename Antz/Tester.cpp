//
//  Tester.cpp
//  Antz
//
//  Created by Zhi Xing on 5/3/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Tester.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Tester::Tester(uint32_t robotId): AntzRobot(robotId) {
}

////////////////////////////////////////////////////////////////
// Setup
void Tester::setup() {
    AntzRobot::setup();
    Serial.begin(9600);
}

////////////////////////////////////////////////////////////////
// Main loop
void Tester::loop() {
    // test motor asynchronous movements
    /*
    goForward(1000);
    delay(3000);
    goBackward(1000);
    delay(3000);
    turnLeft(60);
    delay(3000);
    turnRight(60);
    delay(3000);
    goForward(10000);
    delay(1000);
    goBackward(10000);
    delay(1000);
    turnLeft(360);
    delay(MOTOR_MS_PER_DEG * 60);
    turnRight(360);
    delay(MOTOR_MS_PER_DEG * 60);
    stopMoving();
    delay(1000);
    */
    
    // test scanner
    /*
    float angle;
    Serial.print(" closes distance = ");
    Serial.print(scanner.scan(&angle));
    Serial.print(", degree = ");
    Serial.println(angle);
    */
    
    // test avoid
    //avoid();
}