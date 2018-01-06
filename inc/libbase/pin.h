/*
 * pin.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once
#include "cmsis/SKEAZ1284.h"
#include "misc_utils_c.h"
namespace libbase {

namespace Pin {

enum class Name : uint8_t {
    kPta0 = 0,
    kPta1,
    kPta2,
    kPta3,
    kPta4,
    kPta5,
    kPta6,
    kPta7,

    kPtb0 = 8,
    kPtb1,
    kPtb2,
    kPtb3,
    kPtb4,
    kPtb5,
    kPtb6,
    kPtb7,

    kPtc0 = 16,
    kPtc1,
    kPtc2,
    kPtc3,
    kPtc4,
    kPtc5,
    kPtc6,
    kPtc7,

    kPtd0 = 24,
    kPtd1,
    kPtd2,
    kPtd3,
    kPtd4,
    kPtd5,
    kPtd6,
    kPtd7,

    kPte0 = 32,
    kPte1,
    kPte2,
    kPte3,
    kPte4,
    kPte5,
    kPte6,
    kPte7,

    kPtf0 = 40,
    kPtf1,
    kPtf2,
    kPtf3,
    kPtf4,
    kPtf5,
    kPtf6,
    kPtf7,

    kPtg0 = 48,
    kPtg1,
    kPtg2,
    kPtg3,
    kPtg4,
    kPtg5,
    kPtg6,
    kPtg7,

    kPth0 = 56,
    kPth1,
    kPth2,
    kPth3,
    kPth4,
    kPth5,
    kPth6,
    kPth7,

    kPti0 = 64,
    kPti1,
    kPti2,
    kPti3,
    kPti4,
    kPti5,
    kPti6,

    kDisabled = 71
};

inline uint8_t getPTX(Pin::Name p) { return (uint32_t)p >> 5; }
inline uint8_t getPTN(Pin::Name p) { return (uint32_t)p & 0x1f; }

void PullEnable(Pin::Name, bool enable);

} // namespace Pin

} // namespace libbase
