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
#include "Scanner.h"
#include "Display.h"

#define ID ((uint32_t)8)

using namespace Antz;

Sender sender;
Receiver recver;
Motor motor;
Scanner scanner;
Display display;

// for walker
uint8_t target = 0; // 0 - nest, 1 - food
uint32_t curSource = 0xFFFFFFFF;
uint64_t sourceTime = 0;

// for beacon
uint8_t curFood = 0xFF;
uint8_t curNest = 0xFF;
uint64_t foodTimer = 0;
uint64_t nestTimer = 0;

////////////////////////////////////////////////////////////////
void walker() {
    if (target == 0)
        display.blue(false);
    else if (target == 1)
        display.blue(true);
    
    double angle;
    if (scanner.scan(&angle) <= 15) {
        motor.backward();
        delay(500);
        motor.turnLeft();
        delay(500);
        while (scanner.scan(&angle) <= 15)
            motor.turnLeft();
        motor.forward();
        delay(500);
    }
    
    uint8_t min = 0xFF;
    uint8_t index = 0;
    uint32_t minNumber = 0xFFFFFFFF;
    
    for (int i = 0; i < 6; ++i) {
        uint32_t number;
        if (recver.recvFrom(i, &number)) {
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            if (cardinality == 1)
                target = 1 - target;
            else if (cardinality > 0 && cardinality < min) {
                min = cardinality;
                index = i;
                minNumber = number;
            }
        }
    }
    
    uint8_t cur = target == 0 ? curSource : (curSource >> 8);
    
    if (min <= cur || millis() - sourceTime > 7000) {
        display.number(true, min);
        curSource = minNumber;
        sourceTime = millis();
        if (index == 0)
            motor.forward();
        else if (index < 3)
            motor.turnRight(index * 60);
        else
            motor.turnLeft((6 - index) * 60);
    }
    else // min > cur && millis() - sourceTime <= 3000
        motor.forward();
}

////////////////////////////////////////////////////////////////
void beacon() {
    uint16_t minFood = 0xFF;
    uint16_t minNest = 0xFF;
    
    display.red(true);
    display.green(false);
    bool wait = true;
    while (wait || minFood == 0xFF && minNest == 0xFF) {
        wait = false;
        unsigned long cur = millis();
        if (cur - nestTimer > 15000)
            curNest = 0xFF;
        if (cur - foodTimer > 15000)
            curFood = 0xFF;
        
        for (int i = 0; i < 6; ++i) {
            if (recver.canHearSignal(i)) {
                wait = true;
                uint32_t number;
                if (recver.recvFrom(i, &number)) {
                    uint8_t nest = (uint8_t)(number & 0xFF);
                    uint8_t food = (uint8_t)(number >> 8);
                    if (nest > 0 && nest < minNest)
                        minNest = nest;
                    if (food > 0 && food < minFood)
                        minFood = food;
                }
            }
        }
        if (minNest < (uint16_t)0xFF && minNest + 1 <= curNest) {
            curNest = minNest + 1;
            nestTimer = millis();
        }
        if (minFood < (uint16_t)0xFF && minFood + 1 <= curFood) {
            curFood = minFood + 1;
            foodTimer = millis();
        }
    }
    delay(random(100) * 20);
    if (!recver.canHearSignal()) {
        uint32_t myNumber = (ID << 16) | 0xFFFF;
        myNumber = (myNumber & 0xFF00) | curNest;
        myNumber = (myNumber & 0x00FF) | (curFood << 8);
        display.number(true, curNest);
        display.red(false);
        display.green(true);
        sender.send(myNumber, 2000);
    }
}

////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    scanner.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void loop() {
    beacon();
}
