/*
 * s9keaz128_lqfp80.h
 * Pinout for:
 * S9KEAZ1284
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once

#include "libbase/pin.h"
#include "libbase/adc.h"

namespace libbase {
namespace S9keaz128 {

  Adc::Name GetAdc(const Pin::Name);

}
}
