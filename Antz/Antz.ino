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

//Sender sender;
Receiver recver;
Motor motor;
Scanner scanner;
Display display;

uint8_t target = 0; // 0 - nest, 1 - food
uint32_t curSource = 0xFFFFFFFF;
uint64_t sourceTime = 0;

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
    
    if (min <= cur || millis() - sourceTime > 5000) {
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
/*
 ////////////////////////////////////////////////////////////////
 void beacon() {
 //motor.changeSpeed(0.5);
 //motor.turnLeftInPlace();
 
 //uint16_t id1 = 0;
 //uint16_t id2 = 0;
 //uint16_t id3 = 0;
 //unsigned long cur = millis();
 
 //do {
 uint16_t minFood = 0x00FF;
 uint16_t minNest = 0x00FF;
 unsigned long cur2 = millis();
 do {
 for (int i = 0; i < 6; ++i) {
 uint32_t number = recver.recvFrom(i); // <- problem
 //uint16_t id = (uint16_t)(number >> 16);
 //Serial.println(number);
 //if (id1 == 0 || id1 == id)
 //id1 = id;
 //else if (id2 == 0 || id2 == id)
 //id2 = id;
 //else if (id > 0)
 //id3 = id;
 //else if (id > 0)
 //id2 = id;
 
 uint8_t nest = (uint8_t)(number & 0xFF);
 uint8_t food = (uint8_t)(number >> 8);
 if (nest > 0 && nest < minNest)
 minNest = nest;
 if (food > 0 && food < minFood)
 minFood = food;
 }
 } while (millis() - cur2 < 1000);
 
 Serial.print(" minNest: ");
 Serial.print(minNest);
 Serial.print(" minFood: ");
 Serial.println(minFood);
 
 uint32_t myNumber = (ID << 16) | 0xFFFF;
 if (minNest < (uint16_t)0x00FF)
 myNumber = (myNumber & 0xFF00) | (minNest + 1);
 if (minFood < (uint16_t)0x00FF)
 myNumber = (myNumber & 0x00FF) | ((minFood + 1) << 8);
 //Serial.print(" Send: ");
 //Serial.print(myNumber, HEX);
 //Serial.print(" nest = ");
 //Serial.print(minNest + 1);
 //Serial.print(" food = ");
 //Serial.println(minFood + 1);
 
 display.number(true, (uint8_t)(minNest + 1));
 for (int i = 0; i < 10; ++i)
 sender.send(myNumber, 0);
 //} while (millis() - cur < 5000);
 
 //Serial.print(" id1 = ");
 //Serial.print(id1);
 //Serial.print(" id2 = ");
 //Serial.print(id2);
 //Serial.print(" id3 = ");
 //Serial.println(id3);
 
 //if (id3 != 0)
 //state = 0;
 }
 */
////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    scanner.startup();
    randomSeed(analogRead(0));
}

////////////////////////////////////////////////////////////////
void loop() {
    /*
     if (state == 0) {
     display.number(false, 0);
     if (target == 1)
     display.red(true);
     else if (target == 0)
     display.yellow(true);
     walker();
     display.red(false);
     display.yellow(false);
     }
     else if (state == 1) {
     display.blue(true);
     beacon();
     display.blue(false);
     }
     */
    
    
    //motor.forward();
    /*
    uint8_t min = 0xFF;
    uint8_t index = 0;
    
    for (int i = 0; i < 6; ++i) {
        uint32_t number;
        if (recver.recvFrom(i, &number)) {
            //uint16_t id = (uint16_t)(number >> 16);
            
            //if (id1 == 0 || id1 == id)
            //id1 = id;
            //else if (id > 0)
            //id2 = id;
            
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
    }
    
    display.number(true, min);
    */
    
    walker();
    /*
    for (int i = 0; i < 6; ++i) {
        if (recver.recvFrom(i, NULL)) {
            display.number(true, i);
            delay(500);
            display.number(false);
        }
    }
    */
    //display.number(true, 5);
    //sender.send(0x0105, 0);
    
    /*
    uint32_t signal = 0x0501;
    uint8_t number = signal & 0xFF;
    display.red(true);
    display.green(false);
    display.number(true, number);
    if (!recver.canHearSignal()) {
        delay(random(100) * 20);
        if (!recver.canHearSignal()) {
            display.red(false);
            display.green(true);
            display.number(true, number);
            sender.send(signal, 1000);
        }
    }
     */
}
