/*
 * dir_encoder.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once
#include <stdint.h>
#include "libbase/ftm.h"
#include "libbase/kbi.h"

namespace libsc {

class DirEncoder {
public:
	/*
	 * id:	0 pwm(PTE0)	dir(PTC5)
	 * 		1 pwm(PTE7)	dir(PTH2)
	 */
	DirEncoder(uint8_t id);
	int32_t GetCount();

private:
	static void overFlowListener(libbase::Ftm* ftm);
	static void dirChangeListener(libbase::Kbi* kbi);
	static int32_t count0;
	static int32_t count1;
	static bool dir0;
	static bool dir1;
	int32_t count;
	bool direction;

	libbase::Ftm counter;
	libbase::Kbi dir;
};

}
