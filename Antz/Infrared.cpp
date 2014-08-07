//
//  Infrared.cpp
//  Antz
//
//  Created by Zhi Xing on 7/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Infrared.h"
#include <math.h>

using namespace Antz;

////////////////////////////////////////////////////////////////
Infrared::Infrared(uint8_t nReadings):_nReadings(nReadings)
{
    pinMode(IR_PIN, INPUT);
    if (_nReadings == 0) {
        _nReadings = 1;
    }
}

////////////////////////////////////////////////////////////////
double Infrared::getRange()
{
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
    if (count > 0) {
        distance = sum / count;
    }
    
    return distance;
}