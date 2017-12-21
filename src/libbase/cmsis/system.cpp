/*
 * system.cpp
 *
 * Author: LeeChunHei
 */

#include <stdint.h>
#include "libbase/cmsis/system.h"
#include "libbase/cmsis/SKEAZ1284.h"

volatile uint32_t ics_clk_khz = 0;
volatile uint32_t ftm_clk_khz = 0;
volatile uint32_t core_clk_khz = 0;
volatile uint32_t bus_clk_khz = 0;

void get_clk(void) {
	uint16_t prescaler_factor[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

	if (CLOCK_MODE == 2) {
		ics_clk_khz = 48000;
		ftm_clk_khz = 48000;
		core_clk_khz = 48000;
		bus_clk_khz = 24000;
	} else {
		ics_clk_khz = CRYSTAL_CLK * 1280;
		ics_clk_khz /= prescaler_factor[((ICS->C1 & ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT) + 5];
		ics_clk_khz /= prescaler_factor[(ICS->C2 & ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT];

		core_clk_khz = ics_clk_khz / (((SIM->CLKDIV & SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT) + 1);
		bus_clk_khz = core_clk_khz / (((SIM->CLKDIV & SIM_CLKDIV_OUTDIV2_MASK) >> SIM_CLKDIV_OUTDIV2_SHIFT) + 1);
		ftm_clk_khz = ics_clk_khz / (((SIM->CLKDIV & SIM_CLKDIV_OUTDIV3_MASK) >> SIM_CLKDIV_OUTDIV3_SHIFT) + 1);
	}
}

void SystemInit() {
	//Disable watchdog
	/* WDOG->TOVAL: TOVAL=0xE803 */
	WDOG->TOVAL = WDOG_TOVAL_TOVAL(0xE803); /* Timeout value */
	/* WDOG->CS2: WIN=0,FLG=0,??=0,PRES=0,??=0,??=0,CLK=1 */
	WDOG->CS2 = WDOG_CS2_CLK(0x01); /* 1-kHz clock source */
	/* WDOG->CS1: EN=0,INT=0,UPDATE=1,TST=0,DBG=0,WAIT=1,STOP=1 */
	WDOG->CS1 = WDOG_CS1_UPDATE_MASK | WDOG_CS1_TST(0x00) | WDOG_CS1_WAIT_MASK | WDOG_CS1_STOP_MASK;

	//Set clock
	if (CLOCK_MODE == 2) {
		SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00) | SIM_CLKDIV_OUTDIV2_MASK; //Set system prescalers

		ICS->C2 = ICS_C2_BDIV(0x00) | ICS_C2_LP_MASK;
		ICS->C1 = ICS_C1_CLKS(0x00) | ICS_C1_IREFS_MASK | ICS_C1_IRCLKEN_MASK | ICS_C1_IREFSTEN_MASK;
		while ((ICS->S & ICS_S_IREFST_MASK) == 0x00U) {
		};    //Wait until internal clock selected as reference clock
		while ((ICS->S & ICS_S_LOCK_MASK) == 0x00U) {
		};       //Wait until FLL locked
		while ((ICS->S & 0x0CU) != 0x00U) {
		};                //Wait until output of FLL selected
	} else {
		SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00) | SIM_CLKDIV_OUTDIV2_MASK; //Set system prescalers
		//Switch to PEE mode
		ICS->C2 = ICS_C2_BDIV(0x00) | ICS_C2_LP_MASK;
		OSC->CR = OSC_CR_OSCEN_MASK | OSC_CR_OSCOS_MASK | OSC_CR_RANGE_MASK | OSC_CR_HGO_MASK;
		ICS->C1 = ICS_C1_CLKS(0x00) | ICS_C1_RDIV(0x04 - CLOCK_MODE) | ICS_C1_IRCLKEN_MASK;
		while ((ICS->S & ICS_S_IREFST_MASK) != 0x00U) {
		};    //Wait until internal clock selected as reference clock
		while ((ICS->S & ICS_S_LOCK_MASK) == 0x00U) {
		};       //Wait until FLL locked
		while ((ICS->S & 0x0CU) != 0x00U) {
		}; //Wait until output of FLL selected
	}

//	SIM->SOPT0 = 0x0c; //Disable NMI pin
}


