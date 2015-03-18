//
//  Scanner.h
//  Antz
//
//  Created by Zhi Xing on 7/1/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Scanner__
#define __Antz__Scanner__

#define IR_PIN A15
#define SERVO_PIN     53
#define SERVO_SPD     120.0 // per 60 degrees
#define MIN_POS       10.0
#define MAX_POS       170.0
#define NUM_OF_STOPS  11
#define INTERVAL      (MAX_POS - MIN_POS) / (NUM_OF_STOPS - 1)

#include <Arduino.h>
#include <Servo.h>

namespace Antz {
    class Infrared {
    public:
        Infrared(uint8_t nReadings = 5);
        double getDistance(); // in centimeter(s)
    private:
        uint8_t _nReadings;
    };
    
    class Scanner {
    public:
        Scanner();
        void startup();
        double scan(double* angle);
    private:
        bool forward;
        Servo servo;
        Infrared ir;
    };
}

#endif
