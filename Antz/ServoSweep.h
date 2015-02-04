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
#define SERVO_SPD     120.0 // per 60 degrees
#define MIN_POS       10.0
#define MAX_POS       170.0
#define NUM_OF_STOPS  11
#define INTERVAL      (MAX_POS - MIN_POS) / (NUM_OF_STOPS - 1)

#include <Arduino.h>
#include <Servo.h>
#include "Infrared.h"

namespace Antz {
    class ServoSweep {
    public:
        ServoSweep();
        void startup();
        double sweep(double* angle);
    private:
        bool forward;
        Servo servo;
        Infrared ir;
    };
}

#endif
