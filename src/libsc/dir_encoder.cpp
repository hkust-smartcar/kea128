/*
 * dir_encoder.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libsc/dir_encoder.h"

namespace libsc {

DirEncoder::DirEncoder(uint8_t id) :
		counter(id ? libbase::Ftm::Name::kFTM0 : libbase::Ftm::Name::kFTM1, id ? libbase::Ftm::EXT_CLK::kTCK_2 : libbase::Ftm::EXT_CLK::kTCK_1,
				[this](libbase::Ftm* ftm) {
					count += direction ? ftm->GetCount() : -ftm->GetCount();
					ftm->CleanCount();
				}),
		dir(id ? libbase::Kbi::Name::kKbi1 : libbase::Kbi::Name::kKbi0, libbase::Kbi::Interrupt::kBoth,
				[this](libbase::Kbi* kbi) {
					direction = kbi->GetState();
					count = 0;
					counter.CleanCount();}) {
	dir.initKbi();direction = dir.GetState();
}

int32_t DirEncoder::GetCount() {
	int32_t return_count = count + direction ? counter.GetCount() : -counter.GetCount();
	count = 0;
	counter.CleanCount();
	return return_count;
}

}
