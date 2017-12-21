/*
 * gpio.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/gpio.h"

using libbase::Gpo;
using libbase::Gpi;

Gpi::Gpi(Pin p) {
  ptx = PTX(p);
  ptn = PTn(p);

  RESET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  RESET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

  m_pin = p;
}

bool Gpi::Get() const {
  return ((MEM_MAPS[ptx]->PDIR) >> ptn) & 0x1;
}

Gpo Gpi::ToGpo(bool init) {
  return Gpo(m_pin, init);
}

Gpo::Gpo(Pin p, bool init) {
  ptx = PTX(p);
  ptn = PTn(p);

  SET_BIT(MEM_MAPS[ptx]->PIDR, ptn);
  SET_BIT(MEM_MAPS[ptx]->PDDR, ptn);

  Set(init);

  m_pin = p;
}

void Gpo::Set(bool is_high) {
  if (is_high)
    RESET_BIT(MEM_MAPS[ptx]->PDOR, ptn);
  else
    SET_BIT(MEM_MAPS[ptx]->PDOR, ptn);
}

void Gpo::Turn() {
  SET_BIT(MEM_MAPS[ptx]->PTOR, ptn);
}

Gpi Gpo::ToGpi() {
  return Gpi(m_pin);
}


