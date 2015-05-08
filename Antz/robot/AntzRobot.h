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
#define PROB_TPOS       0.95f
#define PROB_FNEG       0.05f
#define PROB_TNEG       (2.f/3)
#define PROB_FPOS       (1.f/3)
#define SIG_MAXLEN      ((uint32_t)LEN_SIGN + NUM_BITS * (LEN_ONE + LEN_INTR))
#define SIG_PRSV        10000 // in milliseconds

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
        static bool receiveSignal();
        // the six signal should be: front, right front, right rear
        //   rear, left rear, and left front
        static void bayesUpdate(bool signals[]);
        static void bayesUpdate();
        static void bayesReset();
        static uint8_t bayesDecision();
        static void isr();
        
        static uint8_t curMinFood; // after each receiveSignal call, store the minimum food signal during the call
        static uint8_t curMinNest; // after each receiveSignal call, store the minimum nest signal during the call
        static uint64_t foodIndex; // the index curMinFood received from
        static uint64_t nestIndex; // the index curMinNest received from
        static uint8_t minFood; // after each receiveSignal call, store the minimum food signal in the past SIG_PRSV time
        static uint8_t minNest; // after each receiveSignal call, store the minimum nest signal in the past SIG_PRSV time
        static uint64_t foodTimer; // start time of current minFood
        static uint64_t nestTimer; // start time of current minNest
        static uint8_t trueMinFood; // minimum food signal of all time
        static uint8_t trueMinNest; // minimum nest signal of all time
        static uint8_t foodDirect; // estimated direction of food
        static uint8_t nestDirect; // estimated direction of nest
        
        static float condProb[6]; // conditional probability for Bayesian model
        static uint32_t identifier;
        static Display display;
        static Receiver recver;
        static Scanner scanner;
        
    private:
        static int64_t motorStartMillis;
        static int64_t motorStopMillis;
        static uint8_t avoidCnt;
        static MoveType curMovement;
        static Motor motor;
        static void directUpdate(MoveType moveType, uint64_t moveDuration);
    };
}
#endif /* defined(__Antz__AntzRobot__) */
