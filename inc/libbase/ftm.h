/*
 * ftm.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once
#include "libbase/pin.h"

namespace libbase {

class Ftm {
public:
	enum struct FTMn {
		kFTM0 = 0, kFTM1, kFTM2 = 2
	};

	enum struct EXT_CLK {
		kTCK_1 = 0, kTCK_2, kDisable = 3
	};
	
	enum struct CHANNEL {
		kCh0 = 0, kCh1, kCh2, kCh3, kDisable = 4
	};

	Ftm(FTMn ftmn, EXT_CLK external_clk);
	~Ftm();
	void InitChannel(CHANNEL ch);

private:
	const FTMn ftmn;
	const EXT_CLK external_clk;
	uint8_t opened_channel;
};

}
