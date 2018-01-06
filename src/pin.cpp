/*
 * pin.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/pin.h"
namespace libbase {
namespace Pin {

void PullEnable(Pin::Name n, bool enable) {
  uint8_t ptx = getPTX(n);
  uint8_t ptn = getPTN(n);
  switch(ptx) {
      case 0:
          if (enable) PORT->PUE0 |= (uint32_t)(1<<ptn);
          else PORT->PUE0 &= ~(uint32_t)(1<<ptn);
      break;

      case 1:
          if (enable) PORT->PUE1 |= (uint32_t)(1<<ptn);
          else PORT->PUE1 &= ~(uint32_t)(1<<ptn);
      break;

      case 2:
          if (enable) PORT->PUE2 |= (uint32_t)(1<<ptn);
          else PORT->PUE2 &= ~(uint32_t)(1<<ptn);
      break;
  }
}

}
}
