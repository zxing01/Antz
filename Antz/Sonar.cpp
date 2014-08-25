//
//  Sonar.cpp
//  Antz
//
//  Created by Zhi Xing on 6/24/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#include "Sonar.h"

using namespace Antz;

////////////////////////////////////////////////////////////////
Sonar::Sonar() {
    pinMode(SONAR_PIN, INPUT);
}

////////////////////////////////////////////////////////////////
double Sonar::getRange() {
    return pulseIn(SONAR_PIN, HIGH) / 5800.0;
}