/*
 * mpu6050.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include "libsc/mpu6050.h"


namespace libsc {
uint8_t MPU6050::reg = 0x68;

MPU6050::MPU6050(I2CMaster::Name n) {
  m_i2c = new I2CMaster(n, 400000);


  m_i2c->WriteReg(reg, Registers::PWR_MGMT_1, 0x00); // awake
  m_i2c->WriteReg(reg, Registers::SMPLRT_DIV, 0x07); // 125Hz sampling rate
  m_i2c->WriteReg(reg, Registers::CONFIG, 0x04);
  m_i2c->WriteReg(reg, Registers::GYRO_CONFIG, 0x18); // Full scale range +- 2000deg /s
  m_i2c->WriteReg(reg, Registers::ACCEL_CONFIG, 0x10); // Full scale range +- 8g
  m_i2c->WriteReg(reg, Registers::User_Control, 0x00);
  m_i2c->WriteReg(reg, Registers::INT_PIN_CFG, 0x02);

}

MPU6050::~MPU6050() {
  delete m_i2c;
}

MPU6050::Triplet_i16 MPU6050::GetAcc() {
  uint8_t temp[6];

  m_i2c->ReadRegs(reg, Registers::ACCEL_XOUT_H, 6, temp);

  MPU6050::Triplet_i16 t;
  t.x = (int16_t)( ( (uint16_t)temp[0]<<8 | temp[1] ) )>>2;
  t.y = (int16_t)( ( (uint16_t)temp[2]<<8 | temp[3] ) )>>2;
  t.z = (int16_t)( ( (uint16_t)temp[4]<<8 | temp[5] ) )>>2;
  return t;
}

MPU6050::Triplet_i16 MPU6050::GetGyro() {
  uint8_t temp[6];

  m_i2c->ReadRegs(reg, Registers::GYRO_XOUT_H, 6, temp);

  MPU6050::Triplet_i16 t;
  t.x = (int16_t)( ( (uint16_t)temp[0]<<8 | temp[1] ) )>>2;
  t.y = (int16_t)( ( (uint16_t)temp[2]<<8 | temp[3] ) )>>2;
  t.z = (int16_t)( ( (uint16_t)temp[4]<<8 | temp[5] ) )>>2;
  return t;
}
}
