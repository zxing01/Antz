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

#define IDX_FRONT       0
#define IDX_RFRONT      1
#define IDX_RREAR       2
#define IDX_REAR        3
#define IDX_LREAR       4
#define IDX_LFRONT      5
#define IDX_NULL        6
#define MTR_MSPERDEG    18

#define TRUE_POS        0.95f
#define FALSE_NEG       0.05f
#define TRUE_NEG        (2.f/3)
#define FALSE_POS       (1.f/3)

namespace Antz {
    class AntzRobot {
    
    public:
        enum MoveType {mt_forward, mt_backward, mt_left, mt_right, mt_stop, mt_unknown};
        virtual ~AntzRobot() {}
        virtual void setup();
        virtual void loop();
        static AntzRobot* createAntzRobot(char* type, uint32_t robotId);
    
    protected:
        AntzRobot(uint32_t robotId);
        static void goForward(uint64_t msecs, bool async = true);
        static void goBackward(uint64_t msecs, bool async = true);
        static void turnLeft(float degree, bool async = true);
        static void turnRight(float degree, bool async = true);
        static void stopMoving();
        static bool avoid();
        // the six signal should be: front, right front, right rear
        //   rear, left rear, and left front
        static void bayesUpdate(bool signals[]);
        static void bayesUpdate();
        static void bayesReset();
        static uint8_t bayesDecision();
        static void randomWalkGo();
        static void randomWalkReset();
        static void isr();
        
        static uint16_t movePhase;
        static uint32_t identifier;
        static int64_t motorStartMillis;
        static int64_t motorStopMillis;
        static float condProb[6];
        static MoveType curMovement;
        static Display display;
        static Motor motor;
        static Receiver recver;
        static Scanner scanner;
    };
}
#endif /* defined(__Antz__AntzRobot__) */
