//
//  AntzBase.cpp
//  Antz
//
//  Created by Zhi Xing on 8/18/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "AntzBase.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
AntzBase::AntzBase() {
    randomSeed(analogRead(0));
    servo.startup();
}

////////////////////////////////////////////////////////////////
void AntzBase::run() {
    
}

////////////////////////////////////////////////////////////////
void AntzBase::goForward() {
    motor.forward();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void AntzBase::turnLeft() {
    motor.turnLeftInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void AntzBase::turnRight() {
    motor.turnRightInPlace();
    delay(550);
    motor.stop();
}