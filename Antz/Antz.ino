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
#include "Scanner.h"

#define ID ((uint32_t)0)

using namespace Antz;

AntzRobot *antz = AntzRobot::createAntzRobot("Tester", ID);

////////////////////////////////////////////////////////////////
void setup() {
    Serial.begin(9600);
    antz->setup();
}

////////////////////////////////////////////////////////////////
void loop() {
    antz->loop();
}
