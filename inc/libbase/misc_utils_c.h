/*
 * misc_utils_c.h
 * Subset of misc utils that could be used with plain C
 *
 * Author: Ming Tsang, Peter Tse (mcreng)
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#define SET_BIT(x, n) ((x) |= 1 << (n))
#define RESET_BIT(x, n) ((x) &= ~(1 << (n)))
#define CLEAR_BIT(x, n) RESET_BIT(x, n)
#define GET_BIT(x, n) (((x) >> (n)) & 1)
#define GET_BITS(x, n, mask) (((x) & (mask)) >> n)
#define PTX(p) { (uint8_t)p >> 5 }
#define PTn(p) { (uint8_t)p & 0x1f }
