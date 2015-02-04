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
#define NUM_BITS        16
// signal length in microseconds
#define LEN_SIGN        2400
#define LEN_ONE         1200
#define LEN_ZERO        600
#define LEN_INTR        600
// preservation time for received data
#define LEN_PRSV        (LEN_INTR * NUM_BITS * 6)

#endif
