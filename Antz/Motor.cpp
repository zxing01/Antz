//
//  Motor.cpp
//  Antz
//
//  Created by Zhi Xing on 6/2/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Motor.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
Motor::Motor() {
    pinMode(TOGGLE, OUTPUT);
    pinMode(LEFT_DIR, OUTPUT);
    pinMode(LEFT_SPD, OUTPUT);
    pinMode(RIGHT_DIR, OUTPUT);
    pinMode(RIGHT_SPD, OUTPUT);
    
    digitalWrite(TOGGLE, HIGH);
}

////////////////////////////////////////////////////////////////
void Motor::forward(unsigned long ms) {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, MAX_SPD);
    analogWrite(RIGHT_SPD, MAX_SPD);
    delay(ms);
    stop();
}

////////////////////////////////////////////////////////////////
void Motor::backward(unsigned long ms) {
    digitalWrite(LEFT_DIR, HIGH);
    digitalWrite(RIGHT_DIR, LOW);
    analogWrite(LEFT_SPD, MAX_SPD);
    analogWrite(RIGHT_SPD, MAX_SPD);
    delay(ms);
    stop();
}

////////////////////////////////////////////////////////////////
void Motor::turnLeft() {
    digitalWrite(LEFT_DIR, HIGH);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, MAX_SPD/2);
    analogWrite(RIGHT_SPD, MAX_SPD/2);
}

////////////////////////////////////////////////////////////////
void Motor::turnRight() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, LOW);
    analogWrite(LEFT_SPD, MAX_SPD/2);
    analogWrite(RIGHT_SPD, MAX_SPD/2);
}

////////////////////////////////////////////////////////////////
void Motor::turnLeft(double degree) {
    turnLeft();
    delay(degree * MS_PER_DEGREE);
    stop();
}

////////////////////////////////////////////////////////////////
void Motor::turnRight(double degree) {
    turnRight();
    delay(degree * MS_PER_DEGREE);
    stop();
}

////////////////////////////////////////////////////////////////
void Motor::stop() {
    analogWrite(LEFT_SPD, 0);
    analogWrite(RIGHT_SPD, 0);
}
