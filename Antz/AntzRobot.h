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
#include "TimerThree.h"

#define IDX_FRONT           0
#define IDX_REAR            3
#define IDX_LEFT_FRONT      5
#define IDX_LEFT_REAR       4
#define IDX_RIGHT_FRONT     1
#define IDX_RIGHT_REAR      2
#define MTR_MS_PER_DEG    18

namespace Antz {
    class AntzRobot {
    
    public:
        enum MoveType {mt_forward, mt_backward, mt_left, mt_right, mt_stop};
        virtual ~AntzRobot() {}
        virtual void setup();
        virtual void loop();
        static AntzRobot* createAntzRobot(char* type, uint32_t robotId);
    
    protected:
        AntzRobot(uint32_t robotId);
        static void goForward(uint64_t msecs);
        static void goBackward(uint64_t msecs);
        static void turnLeft(float degree);
        static void turnRight(float degree);
        static void stopMoving();
        static bool avoid();
        static void bayesUpdate(bool (*signals)[6]);
        static void bayesUpdate();
        static void bayesReset();
        static void randomWalkGo();
        static void randomWalkReset();
        static void isr();
        
        static uint32_t identifier;
        static uint16_t movePhase;
        static float likelihood[6];
        static int64_t motorStartMillis;
        static int64_t motorStopMillis;
        static Display display;
        static Motor motor;
        static Receiver recver;
        static Scanner scanner;
        static MoveType curMovement;
    };
}
#endif /* defined(__Antz__AntzRobot__) */
