//
//  Infrared.h
//  Antz
//
//  Created by Zhi Xing on 7/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef ANTZ_INFRARED_H
#define ANTZ_INFRARED_H

#define IR_PIN A15

#include <Arduino.h>

namespace Antz {
    class Infrared
    {
    public:
        Infrared(uint8_t nReadings = 5);
        double getRange(); // in centimeter(s)
    private:
        uint8_t _nReadings;
    };
}

#endif /* defined(__Antz__Infrared__) */
