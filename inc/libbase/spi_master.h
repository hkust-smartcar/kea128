/*
 * i2c_master.h
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once
#include <cstddef>
#include "libbase/cmsis/SKEAZ1284.h"

namespace libbase {

class SpiMaster {
public:
  /**
   * SPI0_SCK: B2; SPI0_MOSI: B3; SPI0_MISO: B4; SPI0_PCS: B5
   * SPI1_SCK: G4; SPI1_MOSI: G5; SPI1_MISO: G6; SPI1_PCS: G7
   */
  enum struct Name { kSPI0, kSPI1 };

  SpiMaster(Name, uint32_t baud);
  ~SpiMaster() { Uninit(); };

  /**
   * Exchange bytes
   * @param len Number of bytes to exchange
   * @param out Array of bytes sending out
   * @param in Array to receive byte sending in
   */
  void Exchange(size_t len, uint8_t* out, uint8_t* in = nullptr);

private:
  SPI_Type* spin;
  uint32_t m_baud;
  void TxWait() { while( !( spin->S & SPI_S_SPTEF_MASK ) ); };
  void RxWait() { while( !( spin->S & SPI_S_SPRF_MASK ) ); };
  void Uninit();
  static uint16_t scaler[9];
  static uint8_t prescaler[8];
};

}
