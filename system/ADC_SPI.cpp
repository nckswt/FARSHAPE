#include "ADC_SPI.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>

ADC_SPI::ADC_SPI( int adcChannel, int spiChannel = 0 ) {
  this->adcChannel = adcChannel;
  this->spiChannel = spiChannel;
  this->configBits = ((SINGLE << SINGLE_DIFF_CONFIG) | 
                     (this->adcChannel)) << SHIFT;
  
  //setup SPI
  if( wiringPiSPISetup(spiChannel, 500000) < 0)
    std::cout << "SPI Setup failed: " << std::endl;
  
  RWBuffer[0] = START_BYTE;
  RWBuffer[1] = this->configBits;
  RWBuffer[2] = 0;
  
}

ADC_SPI::~ADC_SPI() {}

double ADC_SPI::getAverage() {
  
  double averageADCValue = 0;
  
  for ( int i = 0; i < AVERAGING; i++ ) {
    averageADCValue += getSingle();
    usleep(10);
  }
  
  return averageADCValue / AVERAGING;
  
}

int ADC_SPI::getSingle() {
  RWBuffer[1] = this->configBits;
  RWBuffer[2] = 0;
  
  wiringPiSPIDataRW (spiChannel, RWBuffer, 3);
  return (int(int(RWBuffer[1]) << 8) | int(RWBuffer[2]));
}