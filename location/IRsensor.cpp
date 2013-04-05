#include "IRsensor.h"

IRsensor::IRsensor(){
//dummy
}

IRsensor::IRsensor(const int adcChannel, std::string calibrationDataFile){
  this->adcChannel = adcChannel;
  this->calibrationDataFile = calibrationDataFile;
  this->value = 0;
  this->adc = new ADC_SPI(adcChannel, DEFAULT_SPI_CHANNEL);
  
  //Calibrate IR Sensor
  readGP2D12CalibrationProfile();
}

IRsensor::~IRsensor(){

}


void IRsensor::readGP2D12CalibrationProfile(){
  //open calibration profile file
  std::ifstream file;
  file.open(calibrationDataFile.c_str(), std::ios::in);

  //assuming one value for each half-cm from 10cm to 80cm
  int i = 0;
  while (file >> IRDistanceMap[i][1]){
      IRDistanceMap[i][0] = 10 + 0.5*i;
      std::cout << this->IRDistanceMap[i][0] << "cm = " << this->IRDistanceMap[i][1] << std::endl;
      i++;
  }
  file.close();
}

double IRsensor::getValue() {
  value = this->adc->getAverage();
  return value;
}

// This is just a test stub -- remove later
int IRsensor::getSingle() {
  return this->adc->getSingle();
}

double IRsensor::getDistance() {
  double distance;
  
  for (int i = 0; i < 142; i++){
    if (this->value > this->IRDistanceMap[i][1]){
      return (this->IRDistanceMap[i-1][0] + this->IRDistanceMap[i][0])/2;
    }
  }
  return 0;
}


