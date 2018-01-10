/*
 * soft_i2c_master.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once
#include "libbase/gpio.h"

namespace libbase {
class SoftI2CMaster {
public:
  SoftI2CMaster(libbase::Pin::Name sda, libbase::Pin::Name scl);
  ~SoftI2CMaster();
  void WriteReg(uint8_t ID, uint8_t reg, uint8_t data);
  uint8_t ReadReg(uint8_t ID, uint8_t reg) ;
  void ReadRegs(uint8_t ID, uint8_t first_reg, uint8_t count, uint8_t* buf);

private:
  Gpo m_sda;
  Gpo m_scl;

  inline void SDA0() { m_sda.Set(false); }
  inline void SDA1() { m_sda.Set(true); }
  inline void SCL0() { m_scl.Set(false); }
  inline void SCL1() { m_scl.Set(true); }
//  inline void SDA_in() { m_sda_i = m_sda.ToGpi(); }
//  inline void SDA_out() { m_sda = m_sda_i.ToGpo(false); }
  inline void Delay() { volatile uint16_t j = 10; while(j--); }

  /**
   * Send Start
   */
  inline void SendStart();

  /**
   * Send Stop
   */
  inline void SendStop();

  /**
   * Send ACK/NACK
   */
  inline void SendACK(bool isACK);

  /**
   * @return If received ACK, return true; otherwise false
   */
  inline bool WaitACK();

  /**
   * Send byte
   */
  inline void SendByte(uint8_t b);

  /**
   * @param needACK Whether send ACK or NACK after reading byte
   * @return Byte read
   */
  inline uint8_t ReadByte(bool needACK);

};
}
