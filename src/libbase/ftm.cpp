/*
 * ftm.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <assert.h>
#include "libbase/ftm.h"
#include "libbase/cmsis/SKEAZ1284.h"

namespace libbase {

Ftm::Ftm(FTMn ftmn, EXT_CLK external_clk) :
		ftmn(ftmn), external_clk(external_clk) {
	opened_channel = 0;
	SIM->SCGC |= 1 << (SIM_SCGC_FTM0_SHIFT + ftmn);

}

Ftm::~Ftm() {

}

void Ftm::InitChannel(CHANNEL ch) {
	if ((opened_channel & (1 << ch)) || ch == CHANNEL::kDisable)	//return if the channel opened before or it is kDisable
		return;
	assert(ftmn < FTMn::kFTM2);

}

}
