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
#define IR_CALIBRATION_FILE "calibration.txt"

using namespace std; // GET RID OF THIS!

class IRsensor
{
public:
  IRsensor(int adcChannel); //, string calibrationDataFile = "calibration.txt");
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
  string calibrationDataFile;
  double value;

};

#endif
