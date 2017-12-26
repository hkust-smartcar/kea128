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

class SPIMaster {
public:
  /**
   * SPI0_SCK: B2; SPI0_MOSI: B3; SPI0_MISO: B4; SPI0_PCS: B5
   * SPI1_SCK: G4; SPI1_MOSI: G5; SPI1_MISO: G6; SPI1_PCS: G7
   */
  enum struct Name { kSPI0, kSPI1 };

  SPIMaster(Name, uint32_t baud);
  ~SPIMaster();
  void Exchange(uint8_t*, uint8_t*, uint32_t len);

private:
  SPI_Type* spin;
  uint32_t m_baud;
  void TxWait() { while( !( spin->S & SPI_S_SPTEF_MASK ) ); };
  void RxWait() { while( !( spin->S & SPI_S_SPRF_MASK ) ); };
  static uint16_t scaler[9];
  static uint8_t prescaler[8];
};

}
