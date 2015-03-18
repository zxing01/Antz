//
//  Scanner.cpp
//  Antz
//
//  Created by Zhi Xing on 7/1/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Scanner.h"
#include <math.h>

using namespace Antz;

////////////////////////////////////////////////////////////////
Infrared::Infrared(uint8_t nReadings):_nReadings(nReadings) {
    pinMode(IR_PIN, INPUT);
    if (_nReadings == 0)
        _nReadings = 1;
}

////////////////////////////////////////////////////////////////
double Infrared::getDistance() {
    double distance = 10000; // default to 100cm
    double sum = 0;
    uint8_t count = 0;
    
    for (uint8_t i = 0; i < _nReadings; ++i) {
        double volts = analogRead(IR_PIN) * 5.0 / 1024.0;
        if (volts > 0) {
            double temp = 12.186 * pow(volts, -1.0463);
            sum += temp;
            ++count;
        }
    }
    if (count > 0)
        distance = sum / count;
    
    return distance;
}

////////////////////////////////////////////////////////////////
Scanner::Scanner() : forward(true) {}

////////////////////////////////////////////////////////////////
void Scanner::startup() {
    servo.attach(SERVO_PIN);
    servo.write(MAX_POS);
    delay(SERVO_SPD * 3);
}

////////////////////////////////////////////////////////////////
double Scanner::scan(double *angle) {
    double ret = 10000; // max double?
    
    for(int i = 0; i < NUM_OF_STOPS; ++i) {
        int direction = forward ? MAX_POS - i * INTERVAL : MIN_POS + i * INTERVAL;
        servo.write(direction);
        delay(SERVO_SPD / 60 * INTERVAL);
        double read = ir.getDistance();
        
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
