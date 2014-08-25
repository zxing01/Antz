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
Motor::Motor(double speed) : speed_(MAX_SPD * speed) {
    pinMode(TOGGLE, OUTPUT);
    pinMode(LEFT_DIR, OUTPUT);
    pinMode(LEFT_SPD, OUTPUT);
    pinMode(RIGHT_DIR, OUTPUT);
    pinMode(RIGHT_SPD, OUTPUT);
    
    digitalWrite(TOGGLE, HIGH);
}

////////////////////////////////////////////////////////////////
void Motor::changeSpeed(double speed) {
    speed_ = MAX_SPD * speed;
}

////////////////////////////////////////////////////////////////
void Motor::forward() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::backward() {
    digitalWrite(LEFT_DIR, HIGH);
    digitalWrite(RIGHT_DIR, LOW);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::turnLeft() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, 0);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::turnRight() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, 0);
}

////////////////////////////////////////////////////////////////
void Motor::turnLeftInPlace() {
    digitalWrite(LEFT_DIR, HIGH);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::turnRightInPlace() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, LOW);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::deviateLeft() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, speed_ * 3 / 4);
    analogWrite(RIGHT_SPD, speed_);
}

////////////////////////////////////////////////////////////////
void Motor::deviateRight() {
    digitalWrite(LEFT_DIR, LOW);
    digitalWrite(RIGHT_DIR, HIGH);
    analogWrite(LEFT_SPD, speed_);
    analogWrite(RIGHT_SPD, speed_ * 3 / 4);
}

////////////////////////////////////////////////////////////////
void Motor::stop() {
    analogWrite(LEFT_SPD, 0);
    analogWrite(RIGHT_SPD, 0);
}
