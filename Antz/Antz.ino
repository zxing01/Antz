//
// Antz
//
// Antz Framework
// Developed with [embedXcode](http://embedXcode.weebly.com)
//
// Author 		Zhi Xing
// 				Zhi Xing
//
// Date			1/22/15 12:49 PM
// Version		<#version#>
//
// Copyright	© Zhi Xing, 2015
// Licence		<#license#>
//
// See         ReadMe.txt for references
//


#include "Sender.h"
#include "Receiver.h"
#include "Motor.h"
#include "Infrared.h"
#include "ServoSweep.h"

using namespace Antz;

Sender sender;
Receiver recver;
Motor motor;
ServoSweep servo;
Infrared ir;
double leftDistance, rightDistance, closestDistance;

////////////////////////////////////////////////////////////////
void goForward() {
    motor.forward();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void turnLeft() {
    motor.turnLeftInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void turnRight() {
    motor.turnRightInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void setup()
{
    Serial.begin(9600);
    servo.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void loop()
{
    //motor.changeSpeed(0.3);
    //motor.turnLeftInPlace();
    //sender.startup();
    //sender.send(5, 0);
    //delay(random(100));
    
    double angle;
    while (servo.sweep(&angle) <= 10) {
        motor.backward();
        delay(500);
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
    }

    uint32_t max;
    uint8_t max_index;
    recver.getMax(&max, &max_index);

    if (max_index == 0) {
        motor.changeSpeed(1);
        motor.forward();
    }
    else if (max_index < 3) {
        motor.changeSpeed(0.5);
        motor.turnRightInPlace();
    }
    else {
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
    }
}
