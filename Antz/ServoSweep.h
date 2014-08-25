//
//  ServoSweep.h
//  Antz
//
//  Created by Zhi Xing on 7/1/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__ServoSweep__
#define __Antz__ServoSweep__

#define SERVO_PIN     53
#define SERVO_SPD     120 // per 60 degrees
#define MIN_POS       10
#define MAX_POS       170
#define NUM_OF_STOPS  5
#define INTERVAL      (MAX_POS - MIN_POS) / (NUM_OF_STOPS - 1)

#include <Arduino.h>
#include <Servo.h>
#include "Infrared.h"

namespace Antz {
    class ServoSweep {
    public:
        ServoSweep();
        void startup();
        void sweep(double *left, double *right, double* closest);
    private:
        bool forward;
        Servo servo;
        Infrared ir;
    };
}

#endif
