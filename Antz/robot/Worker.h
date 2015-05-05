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
    private:
        uint8_t target;
        uint32_t curSource;
        uint64_t sourceTime;
    };
}

#endif /* defined(__Antz__Worker__) */
