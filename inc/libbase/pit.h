/*
 * pit.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once
#include "libbase/cmsis/SKEAZ1284.h"
namespace libbase {



class Pit {
public:
  enum struct Name : uint8_t {
    kPit0 = 0, kPit1
  };

  Pit(Name, uint32_t cnt, void (*listener)(Pit*));
private:
  Name m_chn;
  void ClrFlag(Name n) { PIT->CHANNEL[(uint8_t)n].TFLG |= PIT_TFLG_TIF_MASK; }
};

namespace {
  void (*pit0_listener)(Pit*);
  void (*pit1_listener)(Pit*);
  Pit *pit0, *pit1;
}
}
