//
//  Antz.cpp
//  Antz
//
//  Created by Zhi Xing on 8/18/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Antz.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
Antz::Antz()
{
    randomSeed(analogRead(0));
    servo.startup();
}

////////////////////////////////////////////////////////////////
void Antz::goForward()
{
    motor.forward();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void Antz::turnLeft()
{
    motor.turnLeftInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void Antz::turnRight()
{
    motor.turnRightInPlace();
    delay(550);
    motor.stop();
}