//
//  Sender.h
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef ANTZ_SENDER_H
#define ANTZ_SENDER_H

#define SYSCLOCK        16000 // in KHz
#define IR_KHZ          56
#define TIMER_PWM_PIN   12

#include <avr/interrupt.h>
#include <Arduino.h>
#include "Communication.h"

namespace Antz {
    class Sender
    {
    public:
        Sender();
        void send(uint32_t data, uint32_t duration);
        void startup();
        void shutdown();
        bool isOn();
    private:
        void signal(uint32_t time);
        void interval(uint32_t time);
        bool on;
    };
}

#endif
