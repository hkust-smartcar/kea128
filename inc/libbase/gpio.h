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

namespace Gpio {

enum class Name : uint8_t {
  kPtc6 = 22,
  kPtc7,

  kPtd2 = 26,
  kPtd3,
  kPtd4,
  kPtd5,
  kPtd6,
  kPtd7,

  kPte1 = 33,
  kPte2,
  kPte3,
  kPte4,
  kPte5,
  kPte6,

  kPtf0 = 40,
  kPtf1,

  kPtg0 = 48,
  kPtg1,
  kPtg2,
  kPtg3,

  kPth5 = 61,
  kPth6,
  kPth7,

  kPti3 = 67,
  kPti4,
  kPti5,
  kPti6,

  kDisabled = 71
};

}

class Gpo;

class Gpi {

public:
  Gpi(Pin::Name);
  Gpi(Gpio::Name);
  ~Gpi() { Uninit(); };

  /**
   * Get GPI signal
   */
  bool Get() const;

  /**
   * Convert to GPO
   */
  Gpo&& ToGpo(bool init);

  Gpio::Name GetPin() const { return m_pin; }

private:
  void Uninit();
  Gpio::Name m_pin;
  uint8_t ptx;
  uint8_t ptn;
};

class Gpo {

public:
  Gpo(Pin::Name, bool init);
  Gpo(Gpio::Name, bool init);
  ~Gpo() { Uninit(); };

  /**
   * Set GPO signal output
   */
  void Set(bool is_high);

  /**
   * Toggle GPO signal
   */
  void Turn();

  /**
   * Convert to GPI
   */
  Gpi&& ToGpi();

  Gpio::Name GetPin() { return m_pin; };

private:
  void Uninit();
  Gpio::Name m_pin;
  uint8_t ptx;
  uint8_t ptn;

};


} // namespace libbase
