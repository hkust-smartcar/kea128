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
#include "libbase/misc_utils_c.h"

namespace libbase {

static FTM_Type * const FTMX[] = FTM_BASES;

#define PRECISON 1000u

void (*ftm0_listener)(Ftm *ftm);
void (*ftm1_listener)(Ftm *ftm);
void (*ftm2_listener)(Ftm *ftm);

Ftm *ftm0;
Ftm *ftm1;
Ftm *ftm2;

Ftm::Ftm(Name ftmn, EXT_CLK external_clk, void (*listener)(Ftm*)) :
		ftmn(ftmn), external_clk(external_clk) {
	period = 0;
	opened_channel = 0;
	freq_set = false;
	SIM->SCGC |= 1 << (SIM_SCGC_FTM0_SHIFT + (uint8_t) ftmn);
	if (external_clk != EXT_CLK::kDisable) {
		SIM->PINSEL &= ~1 << (SIM_PINSEL_FTM0CLKPS_SHIFT + (uint8_t) external_clk * 2);
		PORT->PUE1 |= (uint32_t)(1 << ((uint32_t)((external_clk == EXT_CLK::kDisable) ? Pin::Name::kPte0 : Pin::Name::kPte7) & 0x1f));	//pin pull up
		switch (ftmn) {
		case Name::kFTM0:
			ftm0_listener = listener;
			ftm0 = this;
			break;
		case Name::kFTM1:
			ftm1_listener = listener;
			ftm1 = this;
			break;
		case Name::kFTM2:
			ftm2_listener = listener;
			ftm2 = this;
			break;
		}
		if(listener) {
			NVIC_EnableIRQ((IRQn_Type)((uint8_t)ftmn + FTM0_IRQn));
		}
		FTMX[(uint8_t)ftmn]->SC|=(FTM_SC_PS(0) | FTM_SC_CLKS(3) | ((listener) ? FTM_SC_TOIE_MASK : 0));
		FTMX[(uint8_t)ftmn]->CNT=0;
	}
}

Ftm::~Ftm() {

}

void Ftm::InitChannel(CHANNEL ch) {
	if ((opened_channel & (1 << (uint8_t) ch)) || ch == CHANNEL::kDisable)	//return if the channel opened before or it is kDisable
		return;
	assert(ftmn == Name::kFTM2);
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
	assert(ps <= 0x07);
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
	assert(freq_set);
	FTM2->CONTROLS[(uint8_t) ch].CnV = (duty_cycle * (period - 0 + 1)) / PRECISON;
}

void Ftm::SetFreq(uint32_t freq) {
	freq_set = true;
	uint32_t clk_hz = ftm_clk_khz * 1000;
	uint16_t mod = (clk_hz >> 16) / freq;
	uint8_t ps = 0;
	for (; (mod >> ps) >= 1; ++ps) {
	}
	assert(ps <= 0x07);
	mod = (clk_hz >> ps) / freq;
	period = mod;
	FTM2->MOD = mod;
	FTM2->SC = (0 | FTM_SC_PS(ps) | FTM_SC_CLKS(1));
	FTM2->CNTIN = 0;
	FTM2->CNT = 0;
}

uint16_t Ftm::GetCount() {
	return FTMX[(uint8_t) ftmn]->CNT;
}

void Ftm::CleanCount() {
	FTMX[(uint8_t) ftmn]->CNT = 0;
}

void Ftm::TurnCount() {
	if (FTMX[(uint8_t) ftmn])
		if (FTMX[(uint8_t) ftmn]->FMS & FTM_FMS_WPEN_MASK)
			FTMX[(uint8_t) ftmn]->MODE |= FTM_MODE_WPDIS_MASK;
	FTMX[(uint8_t) ftmn]->SC ^= FTM_SC_CPWMS_MASK;
}

extern "C" {
__ISR void FTM0_Handler(void) {
	ftm0_listener(ftm0);
	FTM0->SC ^= FTM_SC_TOF_MASK;
}

__ISR void FTM1_Handler(void) {
	ftm1_listener(ftm1);
	FTM1->SC ^= FTM_SC_TOF_MASK;
}

__ISR void FTM2_Handler(void) {
	ftm2_listener(ftm2);
	FTM2->SC ^= FTM_SC_TOF_MASK;
}
}

}

