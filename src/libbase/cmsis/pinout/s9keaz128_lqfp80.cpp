/*
 * s9keaz128_lqfp80.cpp
 * Pinout for:
 * S9KEAZ1284
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/pinout/s9keaz128_lqfp80.h"

namespace libbase {

namespace S9keaz128 {

Gpio::Name GetGpio(const Pin::Name pin) {
  if ((Pin::Name::kPtc6 <= pin && pin <= Pin::Name::kPtc7)
      || (Pin::Name::kPtd2 <= pin && pin <= Pin::Name::kPtd7)
      || (Pin::Name::kPte1 <= pin && pin <= Pin::Name::kPte6)
      || (Pin::Name::kPtf0 <= pin && pin <= Pin::Name::kPtf1)
      || (Pin::Name::kPtg0 <= pin && pin <= Pin::Name::kPtg3)
      || (Pin::Name::kPth5 <= pin && pin <= Pin::Name::kPth7)
      || (Pin::Name::kPti3 <= pin && pin <= Pin::Name::kPti6)
      )
    return (Gpio::Name)pin;
  else return Gpio::Name::kDisabled;
}

Adc::Name GetAdc(const Pin::Name pin) {

  switch (pin) {

    case Pin::Name::kPta0:
      return Adc::Name::kAdc0Se0;
    case Pin::Name::kPta1:
      return Adc::Name::kAdc0Se1;
    case Pin::Name::kPta6:
      return Adc::Name::kAdc0Se2;
    case Pin::Name::kPta7:
      return Adc::Name::kAdc0Se3;
//    case Pin::Name::kPtb0:
//      return Adc::Name::kAdc0Se4;
//    case Pin::Name::kPtb1:
//      return Adc::Name::kAdc0Se5;
//    case Pin::Name::kPtb2:
//      return Adc::Name::kAdc0Se6;
//    case Pin::Name::kPtb3:
//      return Adc::Name::kAdc0Se7;
    case Pin::Name::kPtc0:
      return Adc::Name::kAdc0Se8;
    case Pin::Name::kPtc1:
      return Adc::Name::kAdc0Se9;
    case Pin::Name::kPtc2:
      return Adc::Name::kAdc0Se10;
    case Pin::Name::kPtc3:
      return Adc::Name::kAdc0Se11;
    case Pin::Name::kPtf4:
      return Adc::Name::kAdc0Se12;
    case Pin::Name::kPtf5:
      return Adc::Name::kAdc0Se13;
    case Pin::Name::kPtf6:
      return Adc::Name::kAdc0Se14;
    case Pin::Name::kPtf7:
      return Adc::Name::kAdc0Se15;
    default:
      return Adc::Name::kDisabled;

  }


}

}
}
