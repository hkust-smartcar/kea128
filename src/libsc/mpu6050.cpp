/*
 * mpu6050.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libsc/mpu6050.h"
#include "libbase/sys_tick.h"

namespace libsc {
uint8_t Mpu6050::reg = 0x68;

Mpu6050::Mpu6050(libbase::I2cMaster::Name n, uint32_t baud) {
//  m_i2c = new I2CMaster(n, 85000);
  mpu = new libbase::I2cMaster(n, baud);

  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::PWR_MGMT_1, 0x00);
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::SMPLRT_DIV, 0x07); // 125Hz sampling rate
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::CONFIG, 0x04);
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::GYRO_CONFIG, 0x18); // Full scale range +- 2000deg /s
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::ACCEL_CONFIG, 0x10); // Full scale range +- 8g
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::User_Control, 0x00);
  libbase::Systick::DelayCycle(1);
  mpu->WriteReg(reg, Registers::INT_PIN_CFG, 0x02);

  libbase::Systick::DelayMs(50);
}

Mpu6050::~Mpu6050() {
  delete mpu;
}

Mpu6050::Triplet_i16 Mpu6050::GetAcc() {
  uint8_t temp[6];

  mpu->ReadRegs(reg, Registers::ACCEL_XOUT_H, 6, temp);

  Mpu6050::Triplet_i16 t;
  t.x = (int16_t)( ( (uint16_t)temp[0]<<8 | temp[1] ) );
  t.y = (int16_t)( ( (uint16_t)temp[2]<<8 | temp[3] ) );
  t.z = (int16_t)( ( (uint16_t)temp[4]<<8 | temp[5] ) );
  return t;
}

Mpu6050::Triplet_i16 Mpu6050::GetGyro() {
  uint8_t temp[6];

  mpu->ReadRegs(reg, Registers::GYRO_XOUT_H, 6, temp);

  Mpu6050::Triplet_i16 t;
  t.x = (int16_t)( ( (uint16_t)temp[0]<<8 | temp[1] ) );
  t.y = (int16_t)( ( (uint16_t)temp[2]<<8 | temp[3] ) );
  t.z = (int16_t)( ( (uint16_t)temp[4]<<8 | temp[5] ) );
  return t;
}
}
