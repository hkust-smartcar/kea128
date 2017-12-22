/*
 * gpio.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "assert.h"
#include "libbase/gpio.h"
#include "libbase/pinout/s9keaz128_lqfp80.h"


using libbase::Gpo;
using libbase::Gpi;

Gpi::Gpi(Pin::Name p) : m_pin(S9keaz128::GetGpio(p)){

  assert(m_pin != Gpio::Name::kDisabled);

  ptx = PTX(p);
  ptn = PTn(p);

  RESET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  RESET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

}

Gpi::Gpi(Gpio::Name p) : m_pin(p){

  ptx = PTX(p);
  ptn = PTn(p);

  RESET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  RESET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

}


bool Gpi::Get() const {
  return ((MEM_MAPS[ptx]->PDIR) >> ptn) & 0x1;
}

Gpo Gpi::ToGpo(bool init) {
  return Gpo(m_pin, init);
}

void Gpi::Uninit() {
  SET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  RESET_BIT(MEM_MAPS[ptx]->PDDR, ptn);
}

Gpo::Gpo(Pin::Name p, bool init) : m_pin(S9keaz128::GetGpio(p)){

  assert(m_pin != Gpio::Name::kDisabled);

  ptx = PTX(p);
  ptn = PTn(p);

  SET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  SET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

  Set(init);

}

Gpo::Gpo(Gpio::Name p, bool init) : m_pin(p){
  ptx = PTX(p);
  ptn = PTn(p);

  SET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  SET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

  Set(init);

}

void Gpo::Set(bool is_high) {
  if (is_high)
    RESET_BIT(MEM_MAPS[ptx]->PDOR, ptn);
  else
    SET_BIT(MEM_MAPS[ptx]->PDOR, ptn);
}

void Gpo::Uninit() {
  SET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  RESET_BIT(MEM_MAPS[ptx]->PDDR, ptn);
}

void Gpo::Turn() {
  SET_BIT(MEM_MAPS[ptx]->PTOR, ptn);
}

Gpi Gpo::ToGpi() {
  return Gpi(m_pin);
}


