/*
 * uart.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once

#include <stdint.h>
#include <functional>

namespace libbase {

class UartDevice {
public:
	/*
	 * Uart0 TX: B1 Uart0 RX: B0
	 * Uart1 TX: F3 Uart1 RX: F2
	 * Uart2 TX: I1 Uart2 RX: I0
	 */
	enum struct Name {
		kUart0 = 0, kUart1, kUart2 = 2
	};

	struct Config {
		Name uartn;
		uint32_t baudrate;
		std::function<void(UartDevice*)> rx_full_listener = nullptr;
		std::function<void(UartDevice*)> tx_empty_listener = nullptr;
	};

	UartDevice(Config config);
	uint8_t GetByte() const;
	void SendBuffer(const uint8_t buff);
	void SendBuffer(const uint8_t* buff, uint16_t buff_length);
	void SendBufferNoWait(const uint8_t buff);

private:
	const Name uartn;
};

}
