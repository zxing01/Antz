//
//  Motor.h
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Motor__
#define __Antz__Motor__

#define TOGGLE    4
#define LEFT_DIR  7
#define LEFT_SPD  9
#define RIGHT_DIR 8
#define RIGHT_SPD 10
#define MAX_SPD   64

#include <Arduino.h>

namespace Antz {
    class Motor {
    public:
        Motor(double speed = 1); // speed is 0 - 1
        void changeSpeed(double speed);
        void forward();
        void backward();
        void turnLeft();
        void turnRight();
        void turnLeftInPlace();
        void turnRightInPlace();
        void deviateLeft();
        void deviateRight();
        void stop();
    private:
        uint8_t speed_;
    };
}

#endif
