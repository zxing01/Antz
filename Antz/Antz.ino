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
bool reach = false;
uint8_t target = 1;

////////////////////////////////////////////////////////////////
void reachTower() {
    if (reach)
        return;
    
    double angle;
    while (servo.sweep(&angle) <= 15) {
        motor.changeSpeed(1);
        motor.backward();
        delay(500);
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
    }
    
    uint32_t max;
    uint8_t max_index;
    //recver.getMax(&max, &max_index);
    
    //Serial.println(max);
    //delay(500);
    
    if (max == 10) {
        reach = true;
        motor.stop();
    }
    else {
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
}

////////////////////////////////////////////////////////////////
void walker() {
    double angle;
    while (servo.sweep(&angle) <= 10) {
        motor.changeSpeed(1);
        motor.backward();
        delay(500);
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
    }
    
    uint8_t min = 0xFF;
    uint8_t index = 0;
    
    for (int i = 0; i < 6; ++i) {
        uint16_t number = recver.recvFrom(i);
        
        Serial.print(" number = ");
        Serial.print(number, BIN);
        Serial.print(" i = ");
        Serial.println(i);
        
        if (target == 0) { // going towards nest
            uint8_t nest = (uint8_t)(number & 0xFF);
            if (nest == 1)
                target = !target;
            else if (nest > 0 && nest < min) {
                min = nest;
                index = i;
            }
        }
        else if (target == 1) {
            uint8_t food = (uint8_t)(number >> 8);
            if (food == 1)
                target = !target;
            else if (food > 0 && food < min) {
                min = food;
                index = i;
            }
        }
    }
    
    Serial.print(" signal = ");
    Serial.print(min);
    Serial.print(" index = ");
    Serial.println(index);
    
    if (index == 0) {
        motor.changeSpeed(1);
        motor.forward();
    }
    else if (index < 3) {
        motor.changeSpeed(0.5);
        motor.turnRightInPlace();
    }
    else {
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
    }
}

////////////////////////////////////////////////////////////////
void beacon() {
    while (1) {
        motor.changeSpeed(0.5);
        motor.turnLeftInPlace();
        uint16_t minFood = 0x00FF;
        uint16_t minNest = 0x00FF;
        unsigned long cur = millis();
        do {
            for (int i = 0; i < 6; ++i) {
                uint16_t number = recver.recvFrom(i);
                uint8_t nest = (uint8_t)(number & 0xFF);
                uint8_t food = (uint8_t)(number >> 8);
                if (nest > 0 && nest < minNest)
                    minNest = nest;
                if (food > 0 && food < minFood)
                    minFood = food;
            }
        } while (millis() - cur < 1000);
        
        Serial.print(" minNest: ");
        Serial.print(minNest);
        Serial.print(" minFood: ");
        Serial.println(minFood);
        
        uint16_t myNumber = 0xFFFF;
        if (minNest < (uint16_t)0x00FF)
            myNumber = (myNumber & 0xFF00) | (minNest + 1);
        if (minFood < (uint16_t)0x00FF)
            myNumber = (myNumber & 0x00FF) | ((minFood + 1) << 8);
        Serial.print(" Send: ");
        Serial.print(myNumber, BIN);
        Serial.print(" nest = ");
        Serial.print(minNest + 1);
        Serial.print(" food = ");
        Serial.println(minFood + 1);
        
        for (int i = 0; i < 50; ++i) {
            sender.send(myNumber, 0);
            delay(random(50));
        }
    }
}

////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    servo.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void loop() {
    walker();
}
