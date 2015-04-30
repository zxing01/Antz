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


#include "AntzRobot.h"

#define ID ((uint32_t)0)

using namespace Antz;

AntzRobot *antz = AntzRobot::createAntzRobot("Walker", ID);

////////////////////////////////////////////////////////////////
void setup() {
    antz->setup();
}

//int counter = 0;
//uint64_t start = 0;
//bool onEdge = false;

////////////////////////////////////////////////////////////////
void loop() {
    antz->loop();
    /*
    int index = 6;
    int idx_arr[] = {3, 2, 4, 1, 5, 0};
    for (int i = 0; i < 6; ++i) {
        if (recver.recvFrom(idx_arr[i], NULL)) {
            display.blue(true);
            if (idx_arr[i] == 3) {
                index = idx_arr[i];
                break;
            }
            else {
                index = idx_arr[i];
            }
        }
        else
            display.blue(false);
    }
    display.number(true, index);
    if (index == 3) {
        if (!onEdge)
            motor.forward(1000);
    }
    else if (index < 3) {
        motor.turnLeft(60 * (3 - index));
    }
    else if (index < 6) {
        motor.turnRight(60 * (index - 3));
    }
    else {
        motor.backward(1000);
        onEdge = true;
    }
    */
    /*
    if (recver.recvFrom(3, NULL)) {
        ++counter;
        display.blue(true);
    }
    else {
        display.blue(false);
    }
    
    if (millis() - start >= 5000) {
        Serial.println(counter);
        start = millis();
        counter = 0;
    }
    */
    /*
    sender.send(0, 0);
    ++counter;
    if (millis() - start >= 5000) {
        Serial.println(counter);
        start = millis();
        counter = 0;
    }
    */
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
}
