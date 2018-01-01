/*
 * flash.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/flash.h"
#include "libbase/cmsis/SKEAZ1284.h"

namespace libbase {

// flash commands
#define ERASE_VERITF_ALL_BLOCKS             0x01  // 擦除检验所有区块
#define ERASE_VERITF_BLOCKS                 0x02  // 擦除检验数据块
#define ERASE_VERITF_FLASH_SECTION          0x03  // 擦除检验Flash 段
#define READ_ONCE                           0x04  // 读取一次
#define PROGRAM_FLASH                       0x06  // 编程Flash
#define PROGRAM_ONCE                        0x07  // 编程一次
#define ERASE_ALL_BLOCKS                    0x08  // 擦除所有区块
#define ERASE_FLASH_BLOCKS                  0x09  // 擦除Flash 区块
#define ERASE_FLASH_SECTOR                  0x0A  // 擦除Flash 扇区
#define UNSECURE_FLASH                      0x0B  // 解密的Flash
#define VERITF_BACKDOOR_ACCESS_KEY          0x0C  // 检验后门访问密钥
#define SET_USER_MARGIN_LEVEL               0x0D  // 设置用户裕量水平
#define SET_FACTORY_MARGIN_LEVEL            0x0E  // 设置出厂裕量水平
#define CONFIGURE_NVM                       0x0F  // 配置NVM

Flash::Flash() {
	SIM->SCGC |= SIM_SCGC_FLASH_MASK;
	while (!(FTMRE->FSTAT & FTMRE_FSTAT_CCIF_MASK))
		;
	FTMRE->FCLKDIV = FTMRE_FCLKDIV_FDIV(bus_clk_khz/1000-1) | FTMRE_FCLKDIV_FDIVLCK_MASK;
	FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;
}

bool Flash::Write(uint32_t sectorNum, uint32_t offset, uint8_t* buff, size_t sizeOfBuff) {
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

	FTMRE->FSTAT = FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_ACCERR_MASK;

	s_flash_run_entry = (flash_run_entry_t)((uint32_t) s_flash_command_run + 1);
	s_flash_run_entry(&FTMRE->FSTAT);

	NVIC_EnableIRQ(FTMRE_IRQn);

	if (FTMRE->FSTAT & (FTMRE_FSTAT_ACCERR_MASK | FTMRE_FSTAT_FPVIOL_MASK | FTMRE_FSTAT_MGSTAT_MASK))
		return false;
	return true;
}

}



