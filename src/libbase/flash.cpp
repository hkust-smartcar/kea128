/*
 * flash.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/flash.h"
#include "libbase/cmsis/SKEAZ1284.h"
#include "libbase/sys_tick.h"

namespace libbase {

#define SECTOR_SIZE 512

// flash commands
#define ERASE_VERITF_ALL_BLOCKS             0x01
#define ERASE_VERITF_BLOCKS                 0x02
#define ERASE_VERITF_FLASH_SECTION          0x03
#define READ_ONCE                           0x04
#define PROGRAM_FLASH                       0x06
#define PROGRAM_ONCE                        0x07
#define ERASE_ALL_BLOCKS                    0x08
#define ERASE_FLASH_BLOCKS                  0x09
#define ERASE_FLASH_SECTOR                  0x0A
#define UNSECURE_FLASH                      0x0B
#define VERITF_BACKDOOR_ACCESS_KEY          0x0C
#define SET_USER_MARGIN_LEVEL               0x0D
#define SET_FACTORY_MARGIN_LEVEL            0x0E
#define CONFIGURE_NVM                       0x0F

volatile uint8_t s_flash_command_run[] = { 0x00, 0xB5, 0x80, 0x21, 0x01, 0x70, 0x01, 0x78, 0x09, 0x06, 0xFC, 0xD5, 0x00, 0xBD };
typedef void (*flash_run_entry_t)(volatile uint8_t *reg);
flash_run_entry_t s_flash_run_entry;

Flash::Flash() {
	SIM->SCGC |= SIM_SCGC_FLASH_MASK;
	while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
		;
	FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(bus_clk_khz/1000-1) | FTMRE_FCLKDIV_FDIVLCK_MASK;
	FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
}

bool Flash::Write(uint32_t sectorNum, uint32_t offset, uint8_t* buff, size_t sizeOfBuff) {
	if (!EraseSector(sectorNum))
		return false;
	uint32_t addr = (uint32_t) sectorNum * SECTOR_SIZE + offset;
	for (uint16_t i = 0; i < sizeOfBuff; i += 4) {
		FTMRE->FCCOBIX = 0;
		FTMRE->FCCOBHI = PROGRAM_FLASH;
		FTMRE->FCCOBLO = addr >> 16;

		FTMRE->FCCOBIX = 1;
		FTMRE->FCCOBHI = (addr & 0xFFFF) >> 8;
		FTMRE->FCCOBLO = addr & 0xFC;

		FTMRE->FCCOBIX = 2;
		FTMRE->FCCOBLO = buff[0];
		FTMRE->FCCOBHI = buff[1];

		FTMRE->FCCOBIX = 3;
		FTMRE->FCCOBLO = buff[2];
		FTMRE->FCCOBHI = buff[3];

		buff += 4;
		addr += 4;

		if (!FlashCmdStart())
			return false;
	}
	return true;
}

bool Flash::FlashCmdStart() {
	NVIC_DisableIRQ(FTMRE_IRQn);
	MCM->PLACR |= MCM_PLACR_ESFC_MASK;
	FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
	s_flash_run_entry = (flash_run_entry_t) ((uint32_t) s_flash_command_run + 1);
	s_flash_run_entry(&FTMRE->FSTAT);
	NVIC_EnableIRQ(FTMRE_IRQn);
	if (FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK))
		return false;
	return true;
}

uint8_t* Flash::Read(uint32_t sectorNum, uint32_t offset, uint16_t buffNeeded) {
	uint32_t temp;
	uint8_t* tempBuff = new uint8_t[buffNeeded];
	for (uint16_t i = 0; i < buffNeeded; i += 4) {
		temp = (*(uint32_t*) (uint32_t)(sectorNum * SECTOR_SIZE + offset + i));
		tempBuff[i + 3] = (uint8_t)(temp >> 24);
		tempBuff[i + 2] = (uint8_t)(temp >> 16);
		tempBuff[i + 1] = (uint8_t)(temp >> 8);
		tempBuff[i] = (uint8_t)(temp);
	}
	return tempBuff;
}

bool Flash::EraseSector(uint32_t sectorNum) {
	uint32_t addr = (uint32_t)(sectorNum * SECTOR_SIZE);

	FTMRE->FCCOBIX = 0;
	FTMRE->FCCOBHI = ERASE_FLASH_SECTOR;
	FTMRE->FCCOBLO = addr >> 16;

	FTMRE->FCCOBIX = 1;
	FTMRE->FCCOBHI = (addr & 0xffff) >> 8;
	FTMRE->FCCOBLO = addr & 0xff;

	return FlashCmdStart();
}

}



