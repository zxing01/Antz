//
//  Receiver.h
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Receiver__
#define __Antz__Receiver__

// state machine states
#define STATE_IDLE      0
#define STATE_SIGN      1
#define STATE_INTR      2
#define STATE_DATA      3
#define STATE_DONE      4
// interrupt pins
#define RECV_PIN0       21
#define RECV_PIN1       20
#define RECV_PIN2       19
#define RECV_PIN3       18
#define RECV_PIN4       2
#define RECV_PIN5       3
// signal tolerance
#define TOLERANCE       0.25
#define LOW_LEN(us)     (int) (us * (1.0 - TOLERANCE))
#define HIGH_LEN(us)    (int) (us * (1.0 + TOLERANCE))

#include <avr/interrupt.h>
#include <Arduino.h>
#include "Communication.h"

namespace Antz {
    class Receiver {
    public:
        struct RecvState {
            RecvState(uint8_t intn, uint8_t iscn0, volatile uint8_t &eicrx)
            :INTn(intn), ISCn0(iscn0), EICRx(eicrx) {}
            uint32_t data;   // output value, only valid when state is STATE_DONE
            uint8_t state;	// state of state machine
            uint8_t bit;    // the bit being received
            uint32_t start;	// start time of last signal
            uint8_t INTn;
            uint8_t ISCn0;
            volatile uint8_t &EICRx;
        };
        
        Receiver();
        uint32_t recvFrom(uint8_t index); // index is 0 - 5, 6 IR receivers, will return 0 if the receiver's not ready or the index is invalid
        
        static void stateTransit(volatile RecvState &recver);
        static volatile RecvState recver0;
        static volatile RecvState recver1;
        static volatile RecvState recver2;
        static volatile RecvState recver3;
        static volatile RecvState recver4;
        static volatile RecvState recver5;
    private:
        uint32_t getData(volatile RecvState &recver);
    };
}

#endif
