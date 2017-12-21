/*
 * sys_tick.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/sys_tick.h"
#include "libbase/cmsis/system.h"

namespace libbase {

volatile uint32_t Systick::m_cur_ticks = 0;

void Systick::Init() {
	get_clk();
	core_clk_khz &= 0x00ffffff;
	StartCount();
}

void Systick::StartCount(uint32_t val) {
	SysTick->LOAD = core_clk_khz / COUNT_LOAD_DIV;
	SysTick->VAL = val;
	SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk);
}

uint32_t Systick::GetTime() {
	return Systick::m_cur_ticks;
}

void Systick::DelayCycle(uint32_t cycle) {
	if (cycle == 0)
		return;

	uint32_t tim = SysTick->VAL;

	SysTick->CTRL = 0x00;
	SysTick->LOAD = cycle;
	SysTick->VAL = 0x00;
	SysTick->CTRL = (0 | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
	while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk))
		;

	StartCount(tim);
}

void Systick::DelayMs(uint32_t ms) {
	if (ms == 0)
		return;

	uint32_t tim = SysTick->VAL;
	while (ms--)
		DelayCycle(core_clk_khz);

	StartCount(tim);
}

extern "C" {
void SysTick_Handler(void) {
	Systick::m_cur_ticks++;
}
}

}
