//
//  Guider.h
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Guider__
#define __Antz__Guider__

#include "AntzRobot.h"

#define DEFAULT_PRIORITY 50

namespace Antz {
    class Guider: public AntzRobot {
    public:
        Guider(uint32_t robotId);
        virtual ~Guider() {}
        virtual void setup();
        virtual void loop();
    protected:
        virtual bool receiveSignal();
        virtual void sendSignal();
        Sender sender;
        uint16_t minFood;
        uint16_t minNest;
        uint8_t curFood;
        uint8_t curNest;
        uint64_t foodTimer;
        uint64_t nestTimer;
        uint8_t priority;
    };
}

#endif /* defined(__Antz__Guider__) */
