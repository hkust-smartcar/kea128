/*
 * kbi.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

namespace libbase {

class Kbi {
public:
	/*
	 * KBI0_P21 PTC5
	 * KBI1_P26 PTH2
	 */
	enum struct Name {
		kKbi0, kKbi1
	};

	enum struct Interrupt {
		kRising, kFalling, kBoth, kFallingLow, kRisingHigh
	};

	Kbi(Name name, Interrupt interrupt, void (*listener)(Kbi*));
	Name GetName() const {
		return kbin;
	}
	Interrupt GetInterrupt() const {
		return interrupt_set;
	}

private:
	Kbi::Name kbin;
	Interrupt interrupt_set;
};

}
