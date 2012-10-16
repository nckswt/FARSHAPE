#include "ADC_SPI.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdio.h>
#include <string>

ADC_SPI::ADC_SPI( int adcChannel, int spiChannel = 0 ) {
  this->adcChannel = adcChannel;
  this->spiChannel = spiChannel;
  this->configBits = ((SINGLE << SINGLE_DIFF_CONFIG) | 
                     (this->adcChannel)) << SHIFT;
  
  // setup wiringPi 
  // TODO: is this required? perhaps do it another file, like a boot up script?
  wiringPiSetup();
  //setup SPI
  if( wiringPiSPISetup(spiChannel, 500000) < 0)
    std::cout << "SPI Setup failed: " << std::endl;
  
}

ADC_SPI::~ADC_SPI() {}

double ADC_SPI::getAverage() {
  
  double averageADCValue = 0;
  
  for ( int i = 0; i < AVERAGING; i++ ) {
    averageADCValue += this->getSingle();
  }
  
  return double(averageADCValue / AVERAGING);
  
}

int ADC_SPI::getSingle() {
  RWBuffer[0] = START_BYTE;
  RWBuffer[1] = this->configBits;
  RWBuffer[2] = 0;
  
  wiringPiSPIDataRW (spiChannel, RWBuffer, 3);

  std::string buffstring((const char)RWBuffer);
  std::cout << buffstring << std::endl; 
  return (int(int(RWBuffer[1]) << 8) | int(RWBuffer[2]));
}
