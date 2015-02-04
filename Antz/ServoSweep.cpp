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
ServoSweep::ServoSweep() : forward(true) {}

////////////////////////////////////////////////////////////////
void ServoSweep::startup() {
    servo.attach(SERVO_PIN);
    servo.write(MAX_POS);
    delay(SERVO_SPD * 3);
}

////////////////////////////////////////////////////////////////
double ServoSweep::sweep(double *angle) {
    double ret = 10000; // max double?
    
    for(int i = 0; i < NUM_OF_STOPS; ++i) {
        int direction = forward ? MAX_POS - i * INTERVAL : MIN_POS + i * INTERVAL;
        servo.write(direction);
        delay(SERVO_SPD / 60 * INTERVAL);
        double read = ir.getRange();
        
        //Serial.println(read);
        
        if (read < ret) {
            ret = read;
            if (angle)
                *angle = direction;
        }
    }
    
    forward = !forward;
    return ret;
}
