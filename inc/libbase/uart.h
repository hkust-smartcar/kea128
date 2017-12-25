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

	Uart(Name uartn, uint32_t &baudrate, void (*rx_full_listener)(Uart*), void (*tx_empty_listener)(Uart*));
	const uint8_t GetByte() const;
	void SendByte(const uint8_t byte);

private:
	const Name uartn;
};

}
