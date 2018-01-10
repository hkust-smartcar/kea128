/*
 * pit.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/pit.h"
#include "libbase/misc_utils_c.h"
namespace libbase {
Pit::Pit(Name n, uint32_t cnt, void (*listener)(Pit*) ) {
  m_chn = n;
  uint8_t chn = (uint8_t)m_chn;
  SIM->SCGC       |= SIM_SCGC_PIT_MASK;
  PIT->MCR         = 0;
  PIT->CHANNEL[chn].LDVAL  = cnt;
  ClrFlag(n);
  PIT->CHANNEL[chn].TCTRL &= ~ PIT_TCTRL_TEN_MASK;
  PIT->CHANNEL[chn].TCTRL  = (PIT_TCTRL_TIE_MASK);

  switch(n) {
  case Name::kPit0:
    pit0_listener = listener;
    pit0 = this;
    break;
  default:
    pit1_listener = listener;
    pit1 = this;
    break;
  }

  NVIC_EnableIRQ( (IRQn_Type)((uint8_t)n + PIT_CH0_IRQn) );
}

extern "C" {
__ISR void PIT_CH0_IRQHandler(void) {
  pit0_listener(pit0);
}

__ISR void PIT_CH1_IRQHandler(void) {
  pit1_listener(pit1);
}

}

}
