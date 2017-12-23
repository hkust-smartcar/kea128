/*
 * adc.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once
#include "pin.h"

namespace libbase {

class Adc {
public:

  enum struct Name {
    kAdc0Se0 = 0,
    kAdc0Se1,
    kAdc0Se2,
    kAdc0Se3,
    kAdc0Se4,
    kAdc0Se5,
    kAdc0Se6,
    kAdc0Se7,
    kAdc0Se8,
    kAdc0Se9,
    kAdc0Se10,
    kAdc0Se11,
    kAdc0Se12,
    kAdc0Se13,
    kAdc0Se14,
    kAdc0Se15,
    kDisabled = 16
  };

  enum struct Bit {
    k8, k10, k12
  };

  Adc(Pin::Name, Bit = Bit::k12);
  ~Adc();
  uint16_t FetchOnce();

  /**
   * Get result in range [0, 3.3]
   */
  float FetchOnceF();

private:
  void Uninit();
  Adc::Name m_pin;
  Bit m_bit;
  static uint8_t m_count;

};
} // namespace libbase
