/*
 * gpio.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "pin.h"

#pragma once

namespace libbase {
namespace {

constexpr GPIO_Type* MEM_MAPS[3] { GPIOA, GPIOB, GPIOC };

}

class Gpo;

class Gpi {

public:
  Gpi(Pin::Name);
  bool Get() const;
  Gpo ToGpo(bool init);
  Pin::Name GetPin() const { return m_pin; }

private:
  void Uninit();
  Pin::Name m_pin;
  uint8_t ptx;
  uint8_t ptn;
};

class Gpo {

public:
  Gpo(Pin::Name, bool init);
  void Set(bool is_high);
  void Turn();
  Gpi ToGpi();
  Pin::Name GetPin() { return m_pin; };

private:
  void Uninit();
  Pin::Name m_pin;
  uint8_t ptx;
  uint8_t ptn;

};


} // namespace libbase
