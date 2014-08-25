//
//  AntzBase.h
//  Antz
//
//  Created by Zhi Xing on 8/18/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__AntzBase__
#define __Antz__AntzBase__

#include "Sender.h"
#include "Receiver.h"
#include "Motor.h"
#include "Infrared.h"
#include "ServoSweep.h"

namespace Antz {
    class AntzBase {
    public:
        AntzBase();
        virtual void run() = 0;
    protected:
        Sender sender;
        Receiver recver;
        Motor motor;
        ServoSweep servo;
        Infrared ir;
        double leftDistance;
        double rightDistance;
        double closestDistance;
        void goForward();
        void turnLeft();
        void turnRight();
    };
}

#endif
