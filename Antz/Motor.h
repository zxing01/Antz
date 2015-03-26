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
#define LEFT_CUR  A0
#define RIGHT_CUR A1

#include <Arduino.h>
#include "Receiver.h"

namespace Antz {
    class Motor {
    public:
        Motor();
        void forward();
        void backward();
        void turnLeft();
        void turnRight();
        void stop();
    private:
        double _msPerDegree;
    };
}

#endif
