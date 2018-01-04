/*
 * flash.h (Not working)
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once

#include <stdint.h>
#include <stddef.h>

namespace libbase {

class Flash {
public:
	Flash();

	/*
	 * @brief Write the buff into corresponding sector with corresponding offset.
	 *
	 * @param sectorNum		the flash sector you want to write in (0-255).
	 *        offset		offset in the flash sector, need to be multiple of 4.
	 *        buff			the data write into flash, total length need to be multiple of 4.
	 *        sizeOfBuff	size of the buff you want to write into flash, need to be multiple of 4.
	 *
	 * @return				true when writing successfully.
	 */
	bool Write(uint32_t sectorNum, uint32_t offset, uint8_t* buff, size_t sizeOfBuff);
	uint8_t* Read(uint32_t sectorNum, uint32_t offset, uint16_t buffNeeded);
	

private:
	bool FlashCmdStart();
	bool EraseSector(uint32_t sectorNum);
};

}
