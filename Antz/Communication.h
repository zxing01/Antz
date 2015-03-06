//
//  Communication.h
//  Antz
//
//  Created by Zhi Xing on 8/6/14.
//  Copyright (c) 2014 Zhi Xing. All rights reserved.
//

#ifndef __Antz__Communication__
#define __Antz__Communication__

// number of bits in a signal
#define NUM_BITS        32
// signal length in microseconds
#define LEN_SIGN        1200
#define LEN_ONE         600
#define LEN_ZERO        300
#define LEN_INTR        300
// preservation time for received data
#define LEN_PRSV        ((uint32_t)LEN_INTR * NUM_BITS * 2)

#endif
