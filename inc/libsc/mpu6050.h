/*
 * mpu6050.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */
#pragma once
#include "libbase/i2c_master.h"

using libbase::I2CMaster;

namespace libsc {

class MPU6050 {
public:
  enum Registers : uint8_t {
     SMPLRT_DIV     =   0x19,  // default 0x07
     CONFIG         =   0x1A,  // default 0x06
     GYRO_CONFIG    =   0x1B,  // default 0x18
     ACCEL_CONFIG   =   0x1C,  // default 0x01
     INT_PIN_CFG    =   0x37,
     ACCEL_XOUT_H   =   0x3B,
     ACCEL_XOUT_L   =   0x3C,
     ACCEL_YOUT_H   =   0x3D,
     ACCEL_YOUT_L   =   0x3E,
     ACCEL_ZOUT_H   =   0x3F,
     ACCEL_ZOUT_L   =   0x40,
     GYRO_XOUT_H    =   0x43,
     GYRO_XOUT_L    =   0x44,
     GYRO_YOUT_H    =   0x45,
     GYRO_YOUT_L    =   0x46,
     GYRO_ZOUT_H    =   0x47,
     GYRO_ZOUT_L    =   0x48,
     User_Control   =   0x6A,
     PWR_MGMT_1     =   0x6B,  // default 0x00
     WHO_AM_I       =   0x75   // default 0x68
  };

  struct Triplet_i16 {
    int16_t x; int16_t y; int16_t z;
  };

  MPU6050(I2CMaster::Name);
  ~MPU6050();
  Triplet_i16 GetGyro();
  Triplet_i16 GetAcc();



private:
  static uint8_t reg;
  I2CMaster* m_i2c;
};
}
