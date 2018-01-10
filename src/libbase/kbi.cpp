/*
 * kbi.cpp
 *
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libbase/kbi.h"
#include "libbase/cmsis/SKEAZ1284.h"
#include "libbase/misc_utils_c.h"

namespace libbase {

volatile KBI_Type * KBIX[2] = { KBI0, KBI1 };

Kbi* kbi0;
Kbi* kbi1;

void (*kbi0_listener)(Kbi*);
void (*kbi1_listener)(Kbi*);

Kbi::Kbi(Name name, Interrupt interrupt, void (*listener)(Kbi*)) {
	interrupt_set = interrupt;
	kbin = name;
	if (interrupt == Interrupt::kBoth)
		interrupt = Interrupt::kFalling;

	SIM->SCGC |= (uint32_t)(1 << (SIM_SCGC_KBI0_SHIFT + (uint8_t) name));

	KBIX[(uint8_t) name]->SC &= ~KBI_SC_KBIE_MASK;

	if ((interrupt == Interrupt::kFalling) || (interrupt == Interrupt::kFallingLow))
		KBIX[(uint8_t) name]->ES &= ~((uint32_t)(1 << (((uint8_t) name) ? 26 : 21)));
	else
		KBIX[(uint8_t) name]->ES |= (uint32_t)(1 << (((uint8_t) name) ? 26 : 21));

	if (name == Name::kKbi0) {
		PORT->PUE0 |= (uint32_t)(1 << 21);
		KBIX[(uint8_t) name]->PE |= (1 << 21);
		KBI0->SC |= (KBI_SC_KBACK_MASK | KBI_SC_RSTKBSP_MASK);
		kbi0 = this;
		kbi0_listener = listener;
		NVIC_EnableIRQ(KBI0_IRQn);
	} else {
		PORT->PUE1 |= (uint32_t)(1 << 26);
		KBIX[(uint8_t) name]->PE |= (1 << 26);
		KBI1->SC |= (KBI_SC_KBACK_MASK | KBI_SC_RSTKBSP_MASK);
		kbi1 = this;
		kbi1_listener = listener;
		NVIC_EnableIRQ(KBI1_IRQn);
	}

	if ((interrupt == Interrupt::kFalling) || (interrupt == Interrupt::kRising))
		KBIX[(uint8_t) name]->SC = 0;
	else
		KBIX[(uint8_t) name]->SC = KBI_SC_KBMOD_MASK;

	KBIX[(uint8_t) name]->SC |= (0 | KBI_SC_KBIE_MASK | KBI_SC_RSTKBSP_MASK | KBI_SC_KBSPEN_MASK);
}

extern "C" {
__ISR void KBI0_IRQHandler(void) {
	KBI0->SC |= KBI_SC_KBACK_MASK;
	kbi0_listener(kbi0);
	if (kbi0->GetInterrupt() == libbase::Kbi::Interrupt::kBoth) {
		KBI0->ES ^= ((uint32_t)(1 << 21));
	}
}

__ISR void KBI1_IRQHandler(void) {
	KBI1->SC |= KBI_SC_KBACK_MASK;
	kbi1_listener(kbi1);
	if (kbi1->GetInterrupt() == libbase::Kbi::Interrupt::kBoth) {
		KBI1->ES ^= ((uint32_t)(1 << 26));
	}
}
}

}
