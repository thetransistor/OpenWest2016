/*
  Filename: bitmacros.h
  Purpose: Misc Bit Math Macros (because the arduino ones aren't as good)
  Created: June 29, 2016
*/

#ifndef BITMACROS_H
#define BITMACROS_H

/* a=target variable, b=bit number to act upon 0-n */
#define BIT_SET(var, bit) ((var) |= (1<<(bit)))
#define BIT_CLR(var, bit) ((var) &= ~(1<<(bit)))
#define BIT_FLIP(var, bit) ((var) ^= (1<<(bit)))
#define BIT_CHECK(var, bit) ((var) & (1<<(bit)))

#endif