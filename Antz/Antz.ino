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

#define ID ((uint32_t)3)

using namespace Antz;

Sender sender;
Receiver recver;
Motor motor;
Scanner scanner;
Display display;

int state = 1; // 0 - beacon, 1 - walker
uint64_t id[5][2] = {0}; // 3 id - starting timestamp pairs

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
    display.red(false);
    display.green(false);
    if (target == 0) { // i'm going towards nest
        display.blue(false);
        display.yellow(true);
    }
    else if (target == 1) {
        display.blue(true);
        display.yellow(false);
    }
    
    double angle;
    if (scanner.scan(&angle) <= 15) { // obstacle avoidance
        motor.backward();
        delay(500); // delay for 500 ms
        motor.turnLeft();
        delay(500);
        while (scanner.scan(&angle) <= 15)
            motor.turnLeft();
        motor.forward();
        delay(500);
    }
    
    // signal format (high to low): 16-bit ID + 8-bit food cardinality + 8-bit nest cardinality
    uint8_t min = 0xFF; // cardinality value I'm following
    uint8_t index = 0; // index of the receiver that receives the signal
    uint32_t minNumber = 0xFFFFFFFF; // 32-bit signal containing the min value
    
    for (int i = 0; i < 6; ++i) { // poll from 6 receivers
        uint32_t number;
        if (recver.recvFrom(i, &number)) {
            uint8_t cardinality = target == 0 ? number : (number >> 8);
            
            uint16_t thisID = (number >> 16);
            for (int i = 0; i < 5; ++i)
                if (id[i][0] == thisID)
                    id[i][0] = 0;
            for (int i = 0; i < 5; ++i) {
                if (id[i][0] == 0) {
                    id[i][0] = thisID;
                    id[i][1] = millis();
                    break;
                }
            }
            
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
        //display.number(true, min);
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
    motor.stop();
    uint16_t minFood = 0xFF; // to store the minimum food cardinality
    uint16_t minNest = 0xFF; // to store the minimum nest cardinality
    
    display.blue(false);
    display.yellow(false);
    display.red(true);
    display.green(false);
    bool wait = true; // a flag indicating whether there're more signals to be heard
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
                uint32_t number; // to store the 32-bit signal
                if (recver.recvFrom(i, &number)) {
                    uint16_t thisID = (number >> 16);
                    for (int i = 0; i < 5; ++i)
                        if (id[i][0] == thisID)
                            id[i][0] = 0;
                    for (int i = 0; i < 5; ++i) {
                        if (id[i][0] == 0) {
                            id[i][0] = thisID;
                            id[i][1] = millis();
                            break;
                        }
                    }
                    
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
        uint32_t myNumber = 0;
        myNumber |= (ID << 16);
        myNumber |= (curFood << 8);
        myNumber |= curNest;
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
    /*
    uint64_t curTime = millis();
    for (int i = 0; i < 5; ++i)
        if (curTime - id[i][1]> 10000)
            id[i][0] = 0;
    
    if (state == 0)
        beacon();
    else if (state == 1)
        walker();
    
    int count = 0;
    for (int i = 0; i < 5; ++i) {
        int n = id[i][0];
        if (id[i][0] > 0)
            ++count;
    }

    display.number(true, count);
    if (state == 0 && count == 3) {
        if (random(100) > 50)
            state = 1;
        else {
            for (int i = 0; i < 5; ++i)
                id[i][0] = 0;
        }
    }
    else if (state == 1 && count < 2) {
        if (random(100) > 50)
            state = 0;
        else {
            for (int i = 0; i < 5; ++i)
                id[i][0] = 0;
        }
    }
    */
    /*
    display.green(true);
    display.red(false);
    sender.send(0x00010501, 2000);
    display.green(false);
    display.red(true);
    delay(2000);
    */
}
