/*
 * math.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/math.h"
namespace libbase {
uint32_t abs(uint32_t value) {
  uint32_t temp = value >> 31;     // make a mask of the sign bit
  value ^= temp;                   // toggle the bits if value is negative
  value += temp & 1;               // add one if value was negative
  return value;
}
}
