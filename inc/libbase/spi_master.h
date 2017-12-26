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
  enum struct Name { kSPI0, kSPI1 };

  SPIMaster(Name, uint32_t baud);
  ~SPIMaster();
  void Exchange(uint8_t*, uint8_t*);

private:
  SPI_Type* spin;
  uint32_t baud;
  void TxWait() { while( !( spin->S & SPI_S_SPTEF_MASK ) ); };
  void RxWait() { while( !( spin->S & SPI_S_SPRF_MASK ) ); };
};

}
