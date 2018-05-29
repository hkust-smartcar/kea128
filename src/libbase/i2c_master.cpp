/*
 * i2c_master.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#include <assert.h>
#include "libbase/math.h"
#include "assert.h"
#include "libbase/i2c_master.h"

namespace libbase {

uint16_t I2cMaster::SCLDivider[0x40]  =
{
    20, 22, 24, 26, 28, 30, 34, 40, 28, 32, 36, 40, 44, 48, 56, 68,
    48, 56, 64, 72, 80, 88, 104, 128, 80, 96, 112, 128, 144, 160, 192, 240,
    160, 192, 224, 256, 288, 320, 384, 480, 320, 384, 448, 512, 576, 640, 768, 960,
    640, 768, 896, 1024, 1152, 1280, 1536, 1920, 1280, 1536, 1792, 2048, 2304, 2560, 3072, 3840
};

I2cMaster::I2cMaster(Name n, uint32_t baud) {
  if (n == Name::kI2C0) {
    SIM->SCGC |= SIM_SCGC_I2C0_MASK;
    SIM->PINSEL &= ~(uint32_t)SIM_PINSEL_I2C0PS_MASK;
    i2cn = I2C0;
  } else {
    SIM->SCGC |= SIM_SCGC_I2C1_MASK;
    SIM->PINSEL1 |= SIM_PINSEL1_I2C1PS_MASK;
    i2cn = I2C1;
  }

  uint8_t mul = bus_clk_khz*1000 / baud / 3840;
  uint16_t scldiv = bus_clk_khz*1000 / ((1<<mul) * baud);
  uint8_t min_diff_id = 0x40, cnt = 0x40;
  uint16_t min_diff = ~0, diff = ~0;

  while (cnt) {
    cnt--;
    diff = libbase::abs(scldiv - SCLDivider[cnt]);
    if (diff == 0) {
      min_diff_id = cnt;
      break;
    }
    else if (diff < min_diff) {
      min_diff_id = cnt;
      min_diff = diff;
    }
  }
  i2cn->F = I2C_F_MULT(mul) | I2C_F_ICR(min_diff_id);
  i2cn->C1 = I2C_C1_IICEN_MASK|0;
//  i2cn->C1 = I2C_C1_IICEN_MASK|I2C_C1_TXAK_MASK;
  m_baud = bus_clk_khz * 1000 / ( (1<<mul) * SCLDivider[min_diff_id]);

  assert( m_baud <= 100000 ); // baud must be less than 100k
}

void I2cMaster::RxWait() {
  uint32_t num = 0;
  while(( i2cn->S1 & I2C_S_IICIF_MASK)==0) {
    num++;
//    libbase::Systick::DelayCycle(100);
    assert (num <= 500); // Time out
  }
  i2cn->S1 |= I2C_S_IICIF_MASK ;
}

uint8_t I2cMaster::ReadReg(uint8_t SlaveID, uint8_t reg) {
  SendStart();
  SetTx((SlaveID << 1) | 0x00);
  SetTx(reg);
  RepeatStart();
  SetTx((SlaveID << 1) | 0x01);

  RxACK();
  uint8_t result = i2cn->D; // initiate read
  RxWait();

  RxNACK();
  result = i2cn->D; // initiate read
  RxWait();
  SendStop();
  result = i2cn->D; // actual read
  Delay(); // mandatory delay
  return result;
}

void I2cMaster::ReadRegs(uint8_t SlaveID, uint8_t first_reg, uint8_t count, uint8_t* buf) {
  SendStart();
  SetTx((SlaveID << 1) | 0x00);
  SetTx(first_reg);
  RepeatStart();
  SetTx((SlaveID << 1) | 0x01);
  RxACK();
  while (count--) {
    *buf = i2cn->D; // initiate read
    RxWait();
    *buf = i2cn->D;
    Delay();
    buf++;
  }
  RxNACK();
  *buf = i2cn->D; // initiate read
  RxWait();
  SendStop();
  *buf = i2cn->D; // actual read
  Delay(); // mandatory delay
}

void I2cMaster::WriteReg(uint8_t SlaveID, uint8_t reg, uint8_t val) {
  SendStart();
  SetTx((SlaveID << 1) | 0x00);
  SetTx(reg);
  SetTx(val);
  SendStop();
  Delay();
}

void I2cMaster::Uninit() {
  if (i2cn == I2C0) {
    SIM->SCGC &= ~SIM_SCGC_I2C0_MASK;
  } else {
    SIM->SCGC &= ~SIM_SCGC_I2C1_MASK;
  }

  i2cn->C1 = 0x00;
}

}
