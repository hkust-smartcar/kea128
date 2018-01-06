/*
 * i2c_master.cpp
 *
 * Author: Peter Tse (mcreng)
 * Copyright (c) 2014-2017 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <assert.h>
#include "libbase/spi_master.h"
#include "libbase/math.h"

namespace libbase {

uint8_t SPIMaster::prescaler[] = {1,2,3,4,5,6,7,8};
uint16_t SPIMaster::scaler[] = {2,4,8,16,32,64,128,256,512};

SPIMaster::SPIMaster(Name n, uint32_t baud) {
  if(n == Name::kSPI0) {
      SIM->SCGC |= SIM_SCGC_SPI0_MASK;
      SIM->PINSEL &= ~(uint32_t)SIM_PINSEL_SPI0PS_MASK;
      spin = SPI0;
  } else {
      SIM->SCGC |= SIM_SCGC_SPI1_MASK;
      SIM->PINSEL1 |= SIM_PINSEL1_SPI1PS_MASK;
      spin = SPI1;
  }

  uint8_t sppr, spr;
  uint32_t min_diff = ~0, diff;
  for(int i=0; i<9; i++) {
      for(int j=0; j<8; j++) {
          diff = libbase::abs(bus_clk_khz*1000/(prescaler[j]*scaler[i]) - baud);
          if(min_diff > diff) {
              sppr = j;
              spr = i;
              min_diff = diff;
              if(min_diff == 0)   break;
          }
      }
  }

  spin->C1 = (SPI_C1_SSOE_MASK | SPI_C1_SPE_MASK | SPI_C1_MSTR_MASK
                    //| SPI_C1_CPOL_MASK
                    //| SPI_C1_CPHA_MASK
                    //| SPI_C1_LSBFE_MASK
                    );
  spin->C2 = SPI_C2_MODFEN_MASK; // auto SS output
  spin->BR = SPI_BR_SPR(spr) | SPI_BR_SPPR(sppr);

  m_baud = (bus_clk_khz*1000/prescaler[sppr]/scaler[spr]);

  assert( m_baud <= bus_clk_khz * 500 ); // baud must be less than bus clock rate / 2
}

void SPIMaster::Exchange(uint8_t* out, uint8_t *in, size_t len) {
  while(len) {
      TxWait();
      spin->D = *out; out++;
      RxWait();
      if(in != nullptr)  { *in = spin->D; in++; }   // actual read
      else               spin->D;                   // virtual read
      len--;
  }
}

void SPIMaster::Uninit() {
  if(spin == SPI0) {
      SIM->SCGC &= ~SIM_SCGC_SPI0_MASK;
  } else {
      SIM->SCGC &= ~SIM_SCGC_SPI1_MASK;
  }
  spin->C1 = spin->C2 = 0x00;
}
}
