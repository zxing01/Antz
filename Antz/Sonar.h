//
//  Sonar.h
//  Antz
//
//  Created by Zhi Xing on 6/24/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Sonar__
#define __Antz__Sonar__

#define SONAR_PIN 52

#include <Arduino.h>

namespace Antz {
    class Sonar {
    public:
        Sonar();
        double getRange(); // in meter(s)
    };
}

#endif /* defined(__Antz__Sonar__) */
