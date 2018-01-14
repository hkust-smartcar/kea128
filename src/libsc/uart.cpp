/*
 * bluetooth.h
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <assert.h>
#include "libsc/uart.h"

using namespace libbase;

UartDevice::Config Uart::GetUartDeviceConfig(Uart::Config config) {
	UartDevice::Config device_config;
	assert(config.id < 3 || config.tx_buffer_size > 0);
	device_config.uartn = (UartDevice::Name) config.id;
	device_config.baudrate = config.baudrate;
	device_config.tx_empty_listener = [this](UartDevice* uart_device) {if(tx_buff.size()) {uart_device->SendBufferNoWait(tx_buff.front());tx_buff.pop();}};
	device_config.rx_full_listener = [this,config](UartDevice* uart_device) {rx_buff.push(uart_device->GetByte());if(rx_buff.size()>config.rx_threshold&&config.rx_isr) {config.rx_isr(rx_buff);while(rx_buff.size()) {rx_buff.pop();};};if(rx_buff.size()>config.rx_threshold) {rx_buff.pop();};};
	return device_config;
}

Uart::Uart(Config config) :
		UartDevice(GetUartDeviceConfig(config)), tx_buffer_size(config.tx_buffer_size) {
}

void Uart::SendBuffer(const uint8_t* buff, uint32_t buff_length) {
	uint32_t index = 1;
	for (; index < tx_buffer_size && index < buff_length && tx_buff.size() < tx_buffer_size; ++index)
		tx_buff.push(buff[index]);
	if (index != 1)
		SendBufferNoWait(buff[0]);
}
