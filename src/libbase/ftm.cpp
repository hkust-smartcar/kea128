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
#include "libbase/cmsis/system.h"

namespace libbase {

#define PRECISON 1000u

Ftm::Ftm(FTMn ftmn, EXT_CLK external_clk) :
		ftmn(ftmn), external_clk(external_clk) {
	period = 0;
	opened_channel = 0;
	freq_set = false;
	SIM->SCGC |= 1 << (SIM_SCGC_FTM0_SHIFT + (uint8_t) ftmn);
	if (external_clk != EXT_CLK::kDisable) {
		SIM->PINSEL &= ~1 << (SIM_PINSEL_FTM0CLKPS_SHIFT + (uint8_t) external_clk * 2);
		PORT->PUE1 |= (uint32_t)(1 << ((uint32_t)((external_clk == EXT_CLK::kDisable) ? Pin::Name::kPte0 : Pin::Name::kPte7) & 0x1f));	//pin pull up
		uint32_t sc = FTM_SC_PS(0) | FTM_SC_CLKS(3);
		switch (ftmn) {
		case FTMn::kFTM0:
			FTM0->SC |= sc;
			FTM0->CNT = 0;
			break;
		case FTMn::kFTM1:
			FTM1->SC |= sc;
			FTM0->CNT = 0;
			break;
		case FTMn::kFTM2:
			FTM2->SC |= sc;
			FTM0->CNT = 0;
			break;
		}
	}
}

Ftm::~Ftm() {

}

void Ftm::InitChannel(CHANNEL ch) {
	if ((opened_channel & (1 << (uint8_t) ch)) || ch == CHANNEL::kDisable)	//return if the channel opened before or it is kDisable
		return;
	assert(ftmn < FTMn::kFTM2);
	SIM->PINSEL1 |= 1 << (SIM_PINSEL1_FTM2PS0_WIDTH * (uint8_t) ch);
	opened_channel |= (1 << (uint8_t) ch);
}

void Ftm::InitOutput(CHANNEL ch, uint32_t freq, uint32_t duty_cycle) {
	if (!(opened_channel & (1 << (uint8_t) ch))) {
		InitChannel(ch);
	}
	freq_set = true;
	uint32_t clk_hz = ftm_clk_khz * 1000;
	uint16_t mod = (clk_hz >> 16) / freq;
	uint8_t ps = 0;
	for (; (mod >> ps) >= 1; ++ps) {
	}
	assert(ps > 0x07);
	mod = (clk_hz >> ps) / freq;
	period = mod;
	//Use FTM2 base ptr only as only FTM2 can set pwm output
	FTM2->MOD = mod;
	FTM2->CONTROLS[(uint8_t) ch].CnSC &= ~FTM_CnSC_ELSA_MASK;
	FTM2->CONTROLS[(uint8_t) ch].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
	FTM2->SC = (0 | FTM_SC_PS(ps) | FTM_SC_CLKS(1));
	FTM2->CNTIN = 0;
	FTM2->CONTROLS[(uint8_t) ch].CnV = (duty_cycle * (mod - 0 + 1)) / PRECISON;
	FTM2->CNT = 0;
}

void Ftm::SetDutyCycle(CHANNEL ch, uint32_t duty_cycle) {
	if (!(opened_channel & (1 << (uint8_t) ch))) {
		InitChannel(ch);
	}
	assert(!freq_set);
	FTM2->CONTROLS[(uint8_t) ch].CnV = (duty_cycle * (period - 0 + 1)) / PRECISON;
}

void Ftm::SetFreq(uint32_t freq) {
	freq_set = true;
	uint32_t clk_hz = ftm_clk_khz * 1000;
	uint16_t mod = (clk_hz >> 16) / freq;
	uint8_t ps = 0;
	for (; (mod >> ps) >= 1; ++ps) {
	}
	assert(ps > 0x07);
	mod = (clk_hz >> ps) / freq;
	period = mod;
	FTM2->MOD = mod;
	FTM2->SC = (0 | FTM_SC_PS(ps) | FTM_SC_CLKS(1));
	FTM2->CNTIN = 0;
	FTM2->CNT = 0;
}

uint16_t Ftm::GetCount() {
	switch (ftmn) {
	case FTMn::kFTM0:
		return FTM0->CNT;
		break;
	case FTMn::kFTM1:
		return FTM1->CNT;
		break;
	case FTMn::kFTM2:
		return FTM2->CNT;
		break;
	}
	return 0;
}

void Ftm::CleanCount() {
	switch (ftmn) {
	case FTMn::kFTM0:
		FTM0->CNT = 0;
		break;
	case FTMn::kFTM1:
		FTM1->CNT = 0;
		break;
	case FTMn::kFTM2:
		FTM2->CNT = 0;
		break;
	}
}

void Ftm::TurnCount() {
	switch (ftmn) {
	case FTMn::kFTM0:
		if (FTM0->FMS & FTM_FMS_WPEN_MASK)
			FTM0->MODE |= FTM_MODE_WPDIS_MASK;
		FTM0->SC ^= FTM_SC_CPWMS_MASK;
		break;
	case FTMn::kFTM1:
		if (FTM1->FMS & FTM_FMS_WPEN_MASK)
			FTM1->MODE |= FTM_MODE_WPDIS_MASK;
		FTM1->SC ^= FTM_SC_CPWMS_MASK;
		break;
	case FTMn::kFTM2:
		if (FTM2->FMS & FTM_FMS_WPEN_MASK)
			FTM2->MODE |= FTM_MODE_WPDIS_MASK;
		FTM2->SC ^= FTM_SC_CPWMS_MASK;
		break;
	}
}

}

