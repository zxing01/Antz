//
//  Guider.cpp
//  Antz
//
//  Created by Zhi Xing on 4/20/15.
//  Copyright (c) 2015 Zhi Xing. All rights reserved.
//

#include "Guider.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
// Contructor
Guider::Guider(uint32_t robotId):
AntzRobot(robotId),
priority(DEFAULT_PRIORITY) {
}

////////////////////////////////////////////////////////////////
// Setup
void Guider::setup() {
    AntzRobot::setup();
}

////////////////////////////////////////////////////////////////
// Main loop
void Guider::loop() {
    stopMoving();
    display.blue(false);
    display.yellow(false);
    display.red(true);
    display.green(false);
    bool wait = true; // a flag indicating whether there're more signals to be heard
    bool noSignal = true;
    while (wait || noSignal) {
        wait = receiveSignal();
        if (wait && noSignal)
            noSignal = false;
    }
    
    delay(random(priority) * 10);
    
    if (!recver.canHearSignal()) {
        priority = DEFAULT_PRIORITY;
        display.red(false);
        display.green(true);
        display.number(true, minFood + 1);
        delay(100);
        display.number(true, minNest + 1);
        sendSignal();
    }
    else if (priority >= 5)
        priority -= 5;
}

////////////////////////////////////////////////////////////////
// Send signals from all the senders
void Guider::sendSignal() {
    uint32_t myNumber = 0;
    myNumber |= (identifier << 16);
    myNumber |= ((uint16_t)(minFood + 1) << 8);
    myNumber |= (minNest + 1);
    sender.send(myNumber, 500);
}
