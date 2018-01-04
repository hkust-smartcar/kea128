/*
 * dir_encoder.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libsc/dir_encoder.h"

namespace libsc {

int32_t DirEncoder::count0 = 0;
int32_t DirEncoder::count1 = 0;
bool DirEncoder::dir0 = true;
bool DirEncoder::dir1 = true;

DirEncoder::DirEncoder(uint8_t id) :
		counter(id ? libbase::Ftm::Name::kFTM0 : libbase::Ftm::Name::kFTM1, id ? libbase::Ftm::EXT_CLK::kTCK_2 : libbase::Ftm::EXT_CLK::kTCK_1, overFlowListener), dir(id ? libbase::Kbi::Name::kKbi1 : libbase::Kbi::Name::kKbi0, libbase::Kbi::Interrupt::kBoth, dirChangeListener) {
}

void DirEncoder::overFlowListener(libbase::Ftm* ftm) {
	if (ftm->GetName() == libbase::Ftm::Name::kFTM0) {
		if (dir1) {
			count1 += ftm->GetCount();
		} else {
			count1 -= ftm->GetCount();
		}
	} else {
		if (dir0) {
			count0 += ftm->GetCount();
		} else {
			count1 -= ftm->GetCount();
		}
	}
}

void DirEncoder::dirChangeListener(libbase::Kbi* kbi) {
	if (kbi->GetName() == libbase::Kbi::Name::kKbi0) {
		FTM1->CNT = 0;
		count0 = 0;
		dir0 = !dir0;
	} else {
		FTM0->CNT = 0;
		count1 = 0;
		dir1 = !dir1;
	}
}

int32_t DirEncoder::GetCount() {
	int32_t count;
	if (dir.GetName() == libbase::Kbi::Name::kKbi0) {
		count = count0 + dir0 ? counter.GetCount() : -counter.GetCount();
		count0 = 0;
	} else {
		count = count1 + dir1 ? counter.GetCount() : -counter.GetCount();
		count1 = 0;
	}
	counter.CleanCount();
	return count;
}

}
