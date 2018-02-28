/*
 * uart.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/uart_device.h"

#include "libbase/cmsis/SKEAZ1284.h"
#include "libbase/misc_utils_c.h"

namespace libbase {

static UART_Type * const UARTX[] = UART_BASES;

UartDevice* uart0;
UartDevice* uart1;
UartDevice* uart2;

std::function<void(UartDevice*)> uart0_rx_full_listener;
std::function<void(UartDevice*)> uart0_tx_empty_listener;
std::function<void(UartDevice*)> uart1_rx_full_listener;
std::function<void(UartDevice*)> uart1_tx_empty_listener;
std::function<void(UartDevice*)> uart2_rx_full_listener;
std::function<void(UartDevice*)> uart2_tx_empty_listener;

UartDevice::UartDevice(Config &config) :
		uartn(config.uartn) {
	SIM->SCGC |= 1 << (SIM_SCGC_UART0_SHIFT + (uint8_t) uartn);
	switch (uartn) {
	case Name::kUart0:
		SIM->PINSEL &= ~SIM_PINSEL_UART0PS_MASK;
		uart0_rx_full_listener = config.rx_full_listener;
		uart0_tx_empty_listener = config.tx_empty_listener;
		uart0 = this;
		break;
	case Name::kUart1:
		SIM->PINSEL1 |= SIM_PINSEL1_UART1PS_MASK;
		uart1_rx_full_listener = config.rx_full_listener;
		uart1_tx_empty_listener = config.tx_empty_listener;
		uart1 = this;
		break;
	case Name::kUart2:
		SIM->PINSEL1 |= SIM_PINSEL1_UART2PS_MASK;
		uart2_rx_full_listener = config.rx_full_listener;
		uart2_tx_empty_listener = config.tx_empty_listener;
		uart2 = this;
		break;
	}
	UARTX[(uint8_t) uartn]->C2 &= ~(0 | UART_C2_TE_MASK | UART_C2_RE_MASK);

	UARTX[(uint8_t) uartn]->C1 |= (0);

	uint32_t uart_input_clk = bus_clk_khz * 1000;

	uint32_t sbr = ((uart_input_clk >> 4) * 10 / config.baudrate + 5) / 10;
	if (sbr > 0x1FFF)
		sbr = 0x1FFF;

	UARTX[(uint8_t) uartn]->BDH &= ~UART_BDH_SBR_MASK;
	UARTX[(uint8_t) uartn]->BDH |= UART_BDH_SBR(sbr >> 8);
	UARTX[(uint8_t) uartn]->BDL = UART_BDL_SBR((uint8_t )sbr);

	UARTX[(uint8_t) uartn]->C2 |= (0 | UART_C2_TE_MASK | UART_C2_RE_MASK);

	if (config.rx_full_listener || config.tx_empty_listener) {
		NVIC_EnableIRQ((IRQn_Type) ((uint8_t) uartn + UART0_IRQn));
	}
	if (config.rx_full_listener) {
		UARTX[(uint8_t) uartn]->C2 |= UART_C2_RIE_MASK;
	}
	if (config.tx_empty_listener) {
		UARTX[(uint8_t) uartn]->C2 |= UART_C2_TCIE_MASK;
	}
}

uint8_t UartDevice::GetByte() const {
	while (!(UARTX[(uint8_t) uartn]->S1 & UART_S1_RDRF_MASK))
		;
	return UARTX[(uint8_t) uartn]->D;
}

void UartDevice::SendBuffer(const uint8_t buff) {
	while (!((UARTX[(uint8_t) uartn]->S1) & UART_S1_TDRE_MASK))
		;
	UARTX[(uint8_t) uartn]->D = buff;
}

void UartDevice::SendBuffer(const uint8_t* buff, uint16_t buff_length) {
	for (uint32_t i = 0; i < buff_length; ++i)
		SendBuffer(buff[i]);
}

void UartDevice::SendBufferNoWait(const uint8_t buff) {
	UARTX[(uint8_t) uartn]->D = buff;
}

extern "C" {
__ISR void UART0_IRQHandler(void) {
	NVIC_DisableIRQ(UART0_IRQn);
	if ((UART0->S1 & UART_S1_TC_MASK) && uart0_tx_empty_listener) {
		uart0_tx_empty_listener(uart0);
		UART0->C2 |= UART_C2_SBK_MASK;
	}
	if ((UART0->S1 & UART_S1_RDRF_MASK) && uart0_rx_full_listener) {
		uart0_rx_full_listener(uart0);
		UART0->D;
	}
	NVIC_EnableIRQ(UART0_IRQn);
}

__ISR void UART1_IRQHandler(void) {
	NVIC_DisableIRQ(UART1_IRQn);
	if ((UART1->S1 & UART_S1_TC_MASK) && uart1_tx_empty_listener) {
		uart1_tx_empty_listener(uart1);
		UART1->C2 |= UART_C2_SBK_MASK;
	}
	if ((UART1->S1 & UART_S1_RDRF_MASK) && uart1_rx_full_listener) {
		uart1_rx_full_listener(uart1);
		UART1->D;
	}
	NVIC_EnableIRQ(UART1_IRQn);
}

__ISR void UART2_IRQHandler(void) {
	NVIC_DisableIRQ(UART2_IRQn);
	if ((UART2->S1 & UART_S1_TC_MASK) && uart2_tx_empty_listener) {
		uart2_tx_empty_listener(uart2);
		UART2->C2 |= UART_C2_SBK_MASK;
	}
	if ((UART2->S1 & UART_S1_RDRF_MASK) && uart2_rx_full_listener) {
		uart2_rx_full_listener(uart2);
		UART2->D;
	}
	NVIC_EnableIRQ(UART2_IRQn);
}
}

}
