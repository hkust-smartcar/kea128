/*
 * i2c_master.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include "libbase/cmsis/SKEAZ1284.h"

namespace libbase {

class I2CMaster {
public:
  /**
   * I2C0_SDA: A2; I2C0_SCL: A3
   * I2C1_SDA: H3, I2C1_SCL: H4
   */
  enum struct Name { kI2C0, kI2C1 };

  I2CMaster(Name, uint32_t baud);
  ~I2CMaster() { Uninit(); };
  uint8_t ReadReg(uint8_t ID, uint8_t reg);
  void ReadRegs(uint8_t ID, uint8_t first_reg, uint8_t count, uint8_t* buf);
  void WriteReg(uint8_t ID, uint8_t reg, uint8_t val);
  uint32_t GetBaud() { return m_baud; }
private:
  I2C_Type* i2cn;
  uint32_t m_baud;
  void SendStart() { i2cn->C1 |= (I2C_C1_TX_MASK | I2C_C1_MST_MASK);}
  void SendStop() { i2cn->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK);}
  void RepeatStart() { i2cn->C1 |= I2C_C1_RSTA_MASK; }
  void RxACK() { i2cn->C1 &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK); }
  void RxNACK() { i2cn->C1 &= ~I2C_C1_TX_MASK; i2cn->C1 |= I2C_C1_TXAK_MASK;}
  void SetNACK() { i2cn->C1 |= I2C_C1_TXAK_MASK; }
  void SetTx(uint8_t byte) {i2cn->D = byte;
  RxWait();}
  void RxWait();
  void Delay() { volatile uint8_t n = 150; while (n--); };
  void Uninit();
  static uint16_t SCLDivider[0x40];
};

}
