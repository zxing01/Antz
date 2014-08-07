
#include "Sender.h"
#include "Receiver.h"
#include "Motor.h"
#include "Infrared.h"
#include "ServoSweep.h"

using namespace Antz;

Sender sender;
Receiver recver;
Motor motor;
ServoSweep servo;
Infrared ir;
double leftDistance, rightDistance, closestDistance;

////////////////////////////////////////////////////////////////
void goForward() {
    motor.forward();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void turnLeft() {
    motor.turnLeftInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void turnRight() {
    motor.turnRightInPlace();
    delay(550);
    motor.stop();
}

////////////////////////////////////////////////////////////////
void setup()
{
    Serial.begin(9600);
    randomSeed(analogRead(0));
    servo.startup();
}

//#define SENDER

////////////////////////////////////////////////////////////////
void loop()
{
#ifdef SENDER
    sender.startup();
    sender.send(0x88, 60000);
#else
    unsigned long r = recver.recvFrom(5);
    Serial.println(r, BIN);
    delay(500);
#endif
    /*
    uint8_t max;
    uint8_t max_index;
    recver.getMax(&max, &max_index);
    
    servo.sweep(&leftDistance, &rightDistance, &closestDistance);
    
    if (closestDistance <= 15) {
        turnLeft();
        if (leftDistance > 15)
            goForward();
    }
    else {
        if (max_index == 0)
            goForward();
        else if (max_index < 3)
            turnRight();
        else
            turnLeft();
    }
     */
}

