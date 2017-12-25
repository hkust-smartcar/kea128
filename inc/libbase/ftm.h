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
	enum struct Name {
		kFTM0 = 0, kFTM1, kFTM2 = 2
	};

	enum struct EXT_CLK {
		kTCK_1 = 0, kTCK_2, kDisable = 3
	};
	
	enum struct CHANNEL {
		kCh0 = 0, kCh1, kCh2, kCh3, kDisable = 4
	};

	Ftm(Name ftmn, EXT_CLK external_clk, void (*listener)(Ftm*));
	~Ftm();
	void InitChannel(CHANNEL ch);
	void InitOutput(CHANNEL ch, uint32_t freq, uint32_t duty_cycle);
	void SetDutyCycle(CHANNEL ch, uint32_t duty_cycle);
	void SetFreq(uint32_t freq);
	uint16_t GetCount();
	void CleanCount();
	void TurnCount();
	
private:
	const Name ftmn;
	const EXT_CLK external_clk;
	uint16_t period;
	uint8_t opened_channel;
	bool freq_set;
};

}
