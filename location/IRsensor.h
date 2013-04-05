#ifndef _IRSENSOR_H_
#define _IRSENSOR_H_

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <map>
#include <fcntl.h>
#include <linux/kd.h>
#include <fstream>
#include "../system/ADC_SPI.h"
 
#define DEFAULT_SPI_CHANNEL 0

class IRsensor
{
public:
  IRsensor();
  IRsensor(const int adcChannel, std::string calibrationDataFile);
  ~IRsensor();
  double getDistance();
  // the below should be made private when the code is considered stable
  double getValue();
  int getSingle();
  
private:
  void readGP2D12CalibrationProfile();

  /* data */
  ADC_SPI* adc;
  double IRDistanceMap[141][2];
  int adcChannel;
  std::string calibrationDataFile;
  double value;

};

#endif
