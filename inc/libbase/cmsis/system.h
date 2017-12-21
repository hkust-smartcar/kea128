/*
 * system.h
 *
 * Author: LeeChunHei
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*
 * CLOCK_MODE 0  Core frequency 40Mhz  Bus frequency 20Mhz  Timer frequency 40Mhz  Uses external crystal
 * CLOCK_MODE 1  Core frequency 80Mhz  Bus frequency 40Mhz  Timer frequency 80Mhz  Uses external crystal
 * CLOCK_MODE 2  Core frequency 48Mhz  Bus frequency 24Mhz  Timer frequency 48Mhz  Uses internal crystal
 */
#define CLOCK_MODE 0
#define CRYSTAL_CLK 16000

void get_clk();
void SystemInit();

extern volatile uint32_t ics_clk_khz;
extern volatile uint32_t ftm_clk_khz;
extern volatile uint32_t core_clk_khz;
extern volatile uint32_t bus_clk_khz;

#ifdef __cplusplus
}
#endif


