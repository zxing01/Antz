//
//  Walker.h
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Walker__
#define __Antz__Walker__

#include "AntzRobot.h"

namespace Antz {
    class Walker: public AntzRobot {
    public:
        Walker(uint32_t robotId);
        virtual ~Walker() {}
        virtual void setup();
        virtual void loop();
    private:
        uint8_t target = 0;
        uint32_t curSource = 0xFFFFFFFF;
        uint64_t sourceTime = 0;
    };
}

#endif /* defined(__Antz__Walker__) */
