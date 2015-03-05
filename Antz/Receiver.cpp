//
//  Receiver.cpp
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Receiver.h"
//#define DEBUG

using namespace Antz;

////////////////////////////////////////////////////////////////
volatile Receiver::RecvState Receiver::recver0(INT0, ISC00, EICRA);
volatile Receiver::RecvState Receiver::recver1(INT1, ISC10, EICRA);
volatile Receiver::RecvState Receiver::recver2(INT2, ISC20, EICRA);
volatile Receiver::RecvState Receiver::recver3(INT3, ISC30, EICRA);
volatile Receiver::RecvState Receiver::recver4(INT4, ISC40, EICRB);
volatile Receiver::RecvState Receiver::recver5(INT5, ISC50, EICRB);

////////////////////////////////////////////////////////////////
Receiver::Receiver() {
    pinMode(RECV_PIN0, INPUT);
    pinMode(RECV_PIN1, INPUT);
    pinMode(RECV_PIN2, INPUT);
    pinMode(RECV_PIN3, INPUT);
    pinMode(RECV_PIN4, INPUT);
    pinMode(RECV_PIN5, INPUT);
    
    EICRA = (1 << ISC01) + (1 << ISC11) + (1 << ISC21) + (1 << ISC31);
    EICRB = (1 << ISC41) + (1 << ISC51);
    // all interrupts are initially disabled
    //EIMSK = (1 << INT0) + (1 << INT1) + (1 << INT2) + (1 << INT3) + (1 << INT4) + (1 << INT5);
}

////////////////////////////////////////////////////////////////
uint32_t Receiver::recvFrom(uint8_t index) {
    switch (index) {
        case 0:
            return getData(recver0);
        case 1:
            return getData(recver1);
        case 2:
            return getData(recver2);
        case 3:
            return getData(recver3);
        case 4:
            return getData(recver4);
        case 5:
            return getData(recver5);
    }
}

////////////////////////////////////////////////////////////////
uint32_t Receiver::getData(volatile RecvState &recver) {
    uint32_t ret = 0;
    EIMSK |= (1 << recver.INTn); // enable interrupt for the receiver
    unsigned long start = micros();
    do {
        if (recver.state == STATE_DONE) {
            EIMSK &= ~(1 << recver.INTn);
            ret = recver.data;
            recver.EICRx &= ~(1 << recver.ISCn0);
            recver.state = STATE_IDLE;
            recver.data = 0;
            recver.bit = 0;
            EIMSK |= (1 << recver.INTn);
        }
        if (ret > 0)
            break;
    } while (micros() - start < LEN_PRSV * 10);
    EIMSK &= ~(1 << recver.INTn); // disable interrupt for the receiver
    return ret;
}

////////////////////////////////////////////////////////////////
void Receiver::stateTransit(volatile RecvState &recver) {
    uint32_t time = micros();
    uint32_t duration = time - recver.start; // duration of signal
    recver.start = time; // record starting time
    
    switch (recver.state) {
        case STATE_IDLE:
            recver.EICRx |= (1 << recver.ISCn0); // change event to raising edge
            recver.state = STATE_SIGN; // receiving signature
            break;
            
        case STATE_SIGN:
            recver.EICRx &= ~(1 << recver.ISCn0); // change event to falling edge
            if (duration >= LOW_LEN(LEN_SIGN) && duration <= HIGH_LEN(LEN_SIGN)) {
                recver.state = STATE_INTR; // received signature, receiving interval
            }
            else {
                recver.state = STATE_IDLE; // go back to idle
                recver.data = 0;
                recver.bit = 0;
            }
            break;
            
        case STATE_INTR:
            recver.EICRx |= (1 << recver.ISCn0);
            if (duration >= LOW_LEN(LEN_INTR) && duration <= HIGH_LEN(LEN_INTR)) {
                recver.state = STATE_DATA; // received interval, receiving data
            }
            else {
                recver.state = STATE_IDLE;
                recver.data = 0;
                recver.bit = 0;
            }
            break;
            
        case STATE_DATA:
            recver.EICRx &= ~(1 << recver.ISCn0);
            if (duration >= LOW_LEN(LEN_ONE) && duration <= HIGH_LEN(LEN_ONE) && recver.bit < NUM_BITS) {
                recver.state = STATE_INTR;
                recver.data |= ((uint32_t)1 << recver.bit++);
            }
            else if (duration >= LOW_LEN(LEN_ZERO) && duration <= HIGH_LEN(LEN_ZERO) && recver.bit < NUM_BITS) {
                recver.state = STATE_INTR;
                recver.data &= ~((uint32_t)1 << recver.bit++);
            }
            else {
                recver.state = STATE_IDLE;
                recver.data = 0;
                recver.bit = 0;
            }
            if (recver.bit == NUM_BITS) {
                recver.state = STATE_DONE;
            }
            break;

        case STATE_DONE:
            if (duration >= LEN_PRSV) {
                recver.EICRx |= (1 << recver.ISCn0);
                recver.state = STATE_SIGN;
                recver.data = 0;
                recver.bit = 0;
            }
            break;
    }
}

////////////////////////////////////////////////////////////////
ISR(INT0_vect) {
    Receiver::stateTransit(Receiver::recver0);
}

////////////////////////////////////////////////////////////////
ISR(INT1_vect) {
    Receiver::stateTransit(Receiver::recver1);
}

////////////////////////////////////////////////////////////////
ISR(INT2_vect) {
    Receiver::stateTransit(Receiver::recver2);
}

////////////////////////////////////////////////////////////////
ISR(INT3_vect) {
    Receiver::stateTransit(Receiver::recver3);
}

////////////////////////////////////////////////////////////////
ISR(INT4_vect) {
    Receiver::stateTransit(Receiver::recver4);
}

////////////////////////////////////////////////////////////////
ISR(INT5_vect) {
    Receiver::stateTransit(Receiver::recver5);
}
