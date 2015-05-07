//
//  Worker.h
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Worker__
#define __Antz__Worker__

#include "AntzRobot.h"

namespace Antz {
    class Worker: public AntzRobot {
    public:
        Worker(uint32_t robotId);
        virtual ~Worker() {}
        virtual void setup();
        virtual void loop();
    protected:
        virtual void receiveSignal();
        virtual void makeMovement();
        uint8_t target;
        uint32_t curNumber;
        uint64_t numberTimer;
        uint8_t minSignal;
        uint8_t signalIndex;
        uint32_t minNumber;
    };
}

#endif /* defined(__Antz__Worker__) */
