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
#include "Display.h"

#define ID ((uint32_t)4)

using namespace Antz;

Sender sender;
Receiver recver;
Motor motor;
ServoSweep servo;
Infrared ir;
Display display;

uint8_t target = 1; // 0 - nest, 1 - food
uint8_t state = 0; // 0 - walker, 1 - beacon

////////////////////////////////////////////////////////////////
void walker() {
    uint16_t id1 = 0;
    uint16_t id2 = 0;
    unsigned long cur = millis();

    do {
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
            uint32_t number = recver.recvFrom(i);
            uint16_t id = (uint16_t)(number >> 16);
            if (id1 == 0 || id1 == id)
                id1 = id;
            else if (id > 0)
                id2 = id;
            
            Serial.print(" number = ");
            Serial.print(number, BIN);
            Serial.print(" i = ");
            Serial.println(i);
            
            if (target == 0) { // going towards nest
                uint8_t nest = (uint8_t)(number & 0xFF);
                if (nest == 1)
                    target = 1;
                else if (nest > 0 && nest < min) {
                    min = nest;
                    index = i;
                }
            }
            else if (target == 1) {
                uint8_t food = (uint8_t)(number >> 8);
                if (food == 1)
                    target = 0;
                else if (food > 0 && food < min) {
                    min = food;
                    index = i;
                }
            }
        }
        
        //Serial.print(" signal = ");
        //Serial.print(min);
        //Serial.print(" index = ");
        //Serial.println(index);
        
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
    } while (millis() - cur < 3000);
    
    if (id2 == 0) // become beacon
        state = 1;
}

////////////////////////////////////////////////////////////////
void beacon() {
    motor.changeSpeed(0.5);
    motor.turnLeftInPlace();

    uint16_t id1 = 0;
    uint16_t id2 = 0;
    uint16_t id3 = 0;
    unsigned long cur = millis();
    
    do {
        uint16_t minFood = 0x00FF;
        uint16_t minNest = 0x00FF;
        unsigned long cur2 = millis();
        do {
            for (int i = 0; i < 6; ++i) {
                uint32_t number = recver.recvFrom(i);
                uint16_t id = (uint16_t)(number >> 16);
                
                if (id1 == 0 || id1 == id)
                    id1 = id;
                else if (id2 == 0 || id2 == id)
                    id2 = id;
                else if (id > 0)
                    id3 = id;
                
                uint8_t nest = (uint8_t)(number & 0xFF);
                uint8_t food = (uint8_t)(number >> 8);
                if (nest > 0 && nest < minNest)
                    minNest = nest;
                if (food > 0 && food < minFood)
                    minFood = food;
            }
        } while (millis() - cur2 < 1000);
        
        //Serial.print(" minNest: ");
        //Serial.print(minNest);
        //Serial.print(" minFood: ");
        //Serial.println(minFood);
        
        uint32_t myNumber = (ID << 16) | 0xFFFF;
        if (minNest < (uint16_t)0x00FF)
            myNumber = (myNumber & 0xFF00) | (minNest + 1);
        if (minFood < (uint16_t)0x00FF)
            myNumber = (myNumber & 0x00FF) | ((minFood + 1) << 8);
        Serial.print(" Send: ");
        Serial.print(myNumber, HEX);
        Serial.print(" nest = ");
        Serial.print(minNest + 1);
        Serial.print(" food = ");
        Serial.println(minFood + 1);
        
        for (int i = 0; i < 50; ++i)
            sender.send(myNumber, 0);
    } while (millis() - cur < 3000);
    
    if (id3 != 0)
        state = 0;
}

////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    servo.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void loop() {
    //if (state == 0)
        //walker();
    //else if (state == 1)
        //beacon();
    //motor.changeSpeed(0.5);
    //motor.turnLeftInPlace();
    //sender.send(0x0001, 0);
    /*
    display.red(true);
    delay(500);
    display.red(false);
    delay(500);
    
    display.green(true);
    delay(500);
    display.green(false);
    delay(500);
    
    display.blue(true);
    delay(500);
    display.blue(false);
    delay(500);
    
    display.yellow(true);
    delay(500);
    display.yellow(false);
    delay(500);
         */
    for (int i = 0; i < 10; ++i) {
        display.number(true, i);
        delay(500);
    }
    
    //delay(500);
    //display.number(false, 8);
    //delay(500);
}
