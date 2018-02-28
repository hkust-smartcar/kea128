/*
 * bluetooth.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#ifndef INC_LIBSC_UART_H_
#define INC_LIBSC_UART_H_

#include "libbase/uart_device.h"
#include <queue>

class Uart: public libbase::UartDevice {
public:
	struct Config {
		uint8_t id;
		uint32_t baudrate;

		/*
		 * Size of the tx buffer byte.
		 * Data that exceed the tx buffer will be popped.
		 */
		uint16_t tx_buffer_size = 16;
		uint16_t rx_threshold = 1;
		std::function<void(std::queue<uint8_t>)> rx_isr = nullptr;
	};

	Uart(Config &config);
	uint16_t GetBuffer(uint8_t* buff);
	void SendBuffer(const uint8_t* buff, uint32_t buff_length);

private:
	libbase::UartDevice::Config& GetUartDeviceConfig(Uart::Config &config);
	std::queue<uint8_t> tx_buff;
	std::queue<uint8_t> rx_buff;
	const uint16_t tx_buffer_size;
};



#endif /* INC_LIBSC_UART_H_ */
