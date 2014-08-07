// 
// Antz 
//
// Framework of Antz Robot
// Developed with [embedXcode](http://embedXcode.weebly.com)
// 
// Author	 	Zhi Xing
// 				Zhi Xing
//
// Date			8/6/14 10:49 PM
// Version		<#version#>
// 
// Copyright	© Zhi Xing, 2014
// License		<#license#>
//
// See			ReadMe.txt for references
//

// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"   
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries
#include "LocalLibrary.h"


// Prototypes


// Define variables and constants
//
// Brief	Name of the LED
// Details	Each board has a LED but connected to a different pin
//
uint8_t myLED;


//
// Brief	Setup
// Details	Define the pin the LED is connected to
//
// Add setup code 
void setup() {
  // myLED pin number
#if defined(ENERGIA) // All LaunchPads supported by Energia
    myLED = RED_LED;
#elif defined(DIGISPARK) // Digispark specific
    myLED = 1; // assuming model A
#elif defined(MAPLE_IDE) // Maple specific
    myLED = BOARD_LED_PIN;
#elif defined(WIRING) // Wiring specific
    myLED = 15;
#else // Arduino, chipKIT, Teensy specific
    myLED = 13;
#endif

    pinMode(myLED, OUTPUT);     
}

//
// Brief	Loop
// Details	Call blink
//
// Add loop code 
void loop() {
    blink(myLED, 3, 333);
    delay(1000);
}
