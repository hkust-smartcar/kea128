/*
 * uart.cpp
 *
 * Author: Leslie Lee (LeeChunHei)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/uart.h"
#include "libbase/cmsis/SKEAZ1284.h"

namespace libbase {

static UART_Type * const UARTX[] = UART_BASES;

Uart::Uart(Name uartn, uint32_t &baudrate, void (*rx_full_listener)(Uart*), void (*tx_empty_listener)(Uart*)) :
		uartn(uartn) {
	SIM->SCGC |= 1 << (SIM_SCGC_UART0_SHIFT + (uint8_t) uartn);
	switch (uartn) {
	case Name::kUart0:
		SIM->PINSEL |= SIM_PINSEL_UART0PS_MASK;
		break;
	case Name::kUart1:
		SIM->PINSEL1 |= SIM_PINSEL1_UART1PS_MASK;
		break;
	case Name::kUart2:
		SIM->PINSEL1 |= SIM_PINSEL1_UART2PS_MASK;
		break;
	}
	UARTX[(uint8_t) uartn]->C2 &= ~(0 | UART_C2_TE_MASK | UART_C2_RE_MASK);

	UARTX[(uint8_t) uartn]->C1 |= (0);

	uint32_t uart_input_clk = bus_clk_khz * 1000;

	uint32_t sbr = ((uart_input_clk >> 4) * 10 / baudrate + 5) / 10;
	if (sbr > 0x1FFF)
		sbr = 0x1FFF;

	UARTX[(uint8_t) uartn]->BDH &= ~UART_BDH_SBR_MASK;
	UARTX[(uint8_t) uartn]->BDH |= UART_BDH_SBR(sbr >> 8);
	UARTX[(uint8_t) uartn]->BDL = UART_BDL_SBR((uint8_t )sbr);

	UARTX[(uint8_t) uartn]->C2 |= (0 | UART_C2_TE_MASK | UART_C2_RE_MASK);
	baudrate = (uart_input_clk >> 4) / sbr;
}

const uint8_t Uart::GetByte() const {
	while (!(UARTX[(uint8_t) uartn]->S1 & UART_S1_RDRF_MASK))
		;
	return UARTX[(uint8_t) uartn]->D;
}

void Uart::SendByte(const uint8_t byte) {
	while (!((UARTX[(uint8_t) uartn]->S1) & UART_S1_TDRE_MASK))
		;
	UARTX[(uint8_t) uartn]->D = byte;
}

}
