//
//  ServoSweep.cpp
//  Antz
//
//  Created by Zhi Xing on 7/1/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "ServoSweep.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
ServoSweep::ServoSweep() : forward(true)
{
}

////////////////////////////////////////////////////////////////
void ServoSweep::startup()
{
    servo.attach(SERVO_PIN);
    servo.write(MAX_POS);
    delay(SERVO_SPD * 3);
}

////////////////////////////////////////////////////////////////
void ServoSweep::sweep(double *left, double *right, double* closest)
{
    *closest = 10000; // max double?
    
#ifdef DEBUG
    Serial.print(" # Antz::ServoSweep --");
    if (forward) {
        Serial.print(" left --");
    }
    else {
        Serial.print(" right --");
    }
#endif
    
    for(int i = 0; i < NUM_OF_STOPS; ++i) {
        if (forward)
            servo.write(MAX_POS - i * INTERVAL);
        else
            servo.write(MIN_POS + i * INTERVAL);
        
        delay(SERVO_SPD / 60 * INTERVAL);
        double read = ir.getRange();
        
#ifdef DEBUG
        Serial.print("\t");
        Serial.print(read);
#endif
        
        if (read < *closest)
            *closest = read;
        if (forward && i == 0 || !forward && i == NUM_OF_STOPS - 1)
            *left = read;
        if (!forward && i == 0 || forward && i == NUM_OF_STOPS - 1)
            *right = read;
    }
    
    forward = !forward;
    
#ifdef DEBUG
    Serial.println();
#endif
}
