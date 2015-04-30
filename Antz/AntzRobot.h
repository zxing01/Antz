//
//  AntzRobot.h
//  AntzRobot
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#ifndef __Antz__AntzRobot__
#define __Antz__AntzRobot__

#include "Display.h"
#include "Motor.h"
#include "Receiver.h"
#include "Scanner.h"
#include "Sender.h"

namespace Antz {
    class AntzRobot {
    public:
        virtual ~AntzRobot() {}
        virtual void setup();
        virtual void loop();
        virtual bool avoid();
        virtual void randomWalkGo();
        virtual void randomWalkReset();
        static AntzRobot* createAntzRobot(char* type, uint32_t robotId);
    protected:
        AntzRobot(uint32_t robotId);
        Display display;
        Motor motor;
        Receiver recver;
        Scanner scanner;
        uint32_t identifier;
        uint8_t movePhase;
    };
}
#endif /* defined(__Antz__AntzRobot__) */
