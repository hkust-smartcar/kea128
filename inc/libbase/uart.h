/*
 * uart.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once

#include <stdint.h>

namespace libbase {

class Uart {
public:
	enum struct Name {
		kUart0 = 0, kUart1, kUart2 = 2
	};

	Uart(Name uartn, uint32_t &baudrate);


private:
	const Name uartn;
};

}
