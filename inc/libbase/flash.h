/*
 * flash.h
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
	volatile uint8_t s_flash_command_run[14] = { 0x00, 0xB5, 0x80, 0x21, 0x01, 0x70, 0x01, 0x78, 0x09, 0x06, 0xFC, 0xD5, 0x00, 0xBD };
	typedef void (*flash_run_entry_t)(volatile uint8_t *reg);
	flash_run_entry_t s_flash_run_entry;
	
	bool FlashCmdStart();
};

}
