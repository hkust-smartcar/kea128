/*
 * adc.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <assert.h>
#include "libbase/adc.h"
#include "libbase/pinout/s9keaz128_lqfp80.h"

namespace libbase {
Adc::Adc(Pin::Name p, Bit b) : m_pin(S9keaz128::GetAdc(p)), m_bit(b) {
  assert(m_pin != Adc::Name::kDisabled);

  SIM->SCGC |= SIM_SCGC_ADC_MASK;
  SET_BIT(ADC->APCTL1, (uint8_t)m_pin);

}

Adc::~Adc() {
  Uninit();
}

uint16_t Adc::FetchOnce() {
  uint16_t result;
  ADC->SC3 = (0
              | ADC_SC3_ADIV(0)
              | ADC_SC3_MODE(m_bit)
              | ADC_SC3_ADICLK(0)
              );

  ADC->SC2 = ADC_SC2_REFSEL(0);

  ADC->SC1 = ADC_SC1_ADCH((uint8_t)m_pin);

  while(!(ADC->SC1 & ADC_SC1_COCO_MASK));
  result = ADC->R;
  return (result & ADC_R_ADR_MASK);
}

void Adc::Uninit() {
  ADC->SC1 = ADC_SC1_ADCH(0xFF);
}

} // namespace libbase
