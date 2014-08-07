//
//  Sonar.h
//  Antz
//
//  Created by Zhi Xing on 6/24/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef ANTZ_SONAR_H
#define ANTZ_SONAR_H

#define SONAR_PIN 52

#include <Arduino.h>

namespace Antz {
    class Sonar
    {
    public:
        Sonar();
        double getRange(); // in meter(s)
    };
}

#endif /* defined(__Antz__Sonar__) */
