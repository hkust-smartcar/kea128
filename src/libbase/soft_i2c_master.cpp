/*
 * soft_i2c_master.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include "libbase/soft_i2c_master.h"
#include "libbase/gpio.h"
namespace libbase {

SoftI2CMaster::SoftI2CMaster(libbase::Pin::Name sda, libbase::Pin::Name scl) :
  m_sda(libbase::Gpo(sda, true)), m_scl(libbase::Gpo(scl, true)) {
  Pin::PullEnable(sda, true);
  Pin::PullEnable(scl, true);
}

SoftI2CMaster::~SoftI2CMaster() {
  Pin::PullEnable((Pin::Name)m_sda.GetPin(), false);
  Pin::PullEnable((Pin::Name)m_scl.GetPin(), false);
}

void SoftI2CMaster::SendStart() {
    SDA1();
    SCL1();
    Delay();
    SDA0();
    Delay();
    SCL0();
}

void SoftI2CMaster::SendStop() {
    SDA0();
    SCL0();
    Delay();
    SCL1();
    Delay();
    SDA1();
    Delay();
}

void SoftI2CMaster::SendACK(bool isACK) {
  SCL0();
  Delay();
  if(isACK) SDA0();
  else      SDA1();
  SCL1();
  Delay();
  SCL0();
  Delay();
}

bool SoftI2CMaster::WaitACK() {
  SCL0();
//  SDA_in();
  Gpi m_sda_i = m_sda.ToGpi();
  Delay();
  SCL1();
  Delay();
  if(m_sda_i.Get())
  {
//      SDA_out();
      m_sda = m_sda_i.ToGpo(false);
      SCL0();
      return false;
  }
//  SDA_out();
  m_sda = m_sda_i.ToGpo(false);
  SCL0();
  Delay();
  return true;
}

void SoftI2CMaster::SendByte(uint8_t b) {
  uint8_t i = 8;
  while(i--)
  {
      if(b & 0x80)    SDA1();
      else            SDA0();
      b <<= 1;
      Delay();
      SCL1();
      Delay();
      SCL0();
  }
  WaitACK();
}

uint8_t SoftI2CMaster::ReadByte(bool needACK) {
  uint8_t c = 0;
  SCL0();
  Delay();
  SDA1();
//  SDA_in();
  Gpi m_sda_i = m_sda.ToGpi();
  for(int i=0;i<8;i++) {
      Delay();
      SCL0();
      Delay();
      SCL1();
      Delay();
      c<<=1;
      if(m_sda_i.Get()) c+=1;
  }
//  SDA_out();
  m_sda = Gpo(m_sda_i.GetPin(), false);
  SCL0();
  Delay();
  SendACK(needACK);

  return c;
}


void SoftI2CMaster::WriteReg(uint8_t ID, uint8_t reg, uint8_t data) {
  SendStart();
  SendByte( (ID<<1) | 0x00 );
  SendByte(reg);
  SendByte(data);
  SendStop();
}

uint8_t SoftI2CMaster::ReadReg(uint8_t ID, uint8_t reg) {
  uint8_t data;
  SendStart();
  SendByte( (ID<<1) | 0x00 );
  SendByte(reg);

  SendStart();
  SendByte( (ID<<1) | 0x01 );
  data = ReadByte(false);
  SendStop();

  return data;
}

void SoftI2CMaster::ReadRegs(uint8_t ID, uint8_t first_reg, uint8_t count, uint8_t* buf) {
  SendStart();
  SendByte( (ID<<1) | 0x00 );
  SendByte(first_reg);

  SendStart();
  SendByte( (ID<<1) | 0x01 );
  while(--count) {
      *buf = ReadByte(true);
      buf++;
  }
  *buf = ReadByte(false);
  SendStop();
}
}
