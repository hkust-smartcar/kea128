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

uint8_t Adc::m_count = 0;

Adc::Adc(Pin::Name p, Bit b) : m_pin(S9keaz128::GetAdc(p)), m_bit(b) {
  assert(m_pin != Adc::Name::kDisabled);

  SIM->SCGC |= SIM_SCGC_ADC_MASK;
  SET_BIT(ADC->APCTL1, (uint8_t)m_pin);

  m_count++;
}

Adc::~Adc() {
  Uninit();
}

uint16_t Adc::FetchOnce() {
  uint16_t result;
  ADC->SC3 = (ADC_SC3_ADIV(0) | ADC_SC3_MODE(m_bit) | ADC_SC3_ADICLK(0));

  ADC->SC2 = ADC_SC2_REFSEL(0);

  ADC->SC1 = ADC_SC1_ADCH((uint8_t)m_pin);

  while(!(ADC->SC1 & ADC_SC1_COCO_MASK));
  result = ADC->R & ADC_R_ADR_MASK;
  ADC->SC1 = ADC_SC1_ADCH(0xFF); // Disable ADC
  return result;
}

float Adc::FetchOnceF() {
  float resultF = FetchOnce() * 3.3f;
  switch (m_bit) {
  case Bit::k8:
    return resultF / 0x00FF;
  case Bit::k10:
    return resultF / 0x02FF;
  default:
    return resultF / 0x0FFF;
  }
}

void Adc::Uninit() {
  if (m_count-- == 0) SIM->SCGC &= ~SIM_SCGC_ADC_MASK; // Disable ADC time
  RESET_BIT(ADC->APCTL1, (uint8_t)m_pin); // Disable ADC pin
}

} // namespace libbase
