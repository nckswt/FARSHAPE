#include "IRsensor.h"

IRsensor::IRsensor(int adcChannel){
  this->adcChannel = adcChannel;
  this->calibrationDataFile = IR_CALIBRATION_FILE;
  this->value = 0;

  this->adc = ADC_SPI(this->adcChannel, DEFAULT_SPI_CHANNEL);
  
  //Calibrate IR Sensor
  readGP2D12CalibrationProfile();
}

IRsensor::~IRsensor(){

}


void IRsensor::readGP2D12CalibrationProfile(){
  //open calibration profile file
  ifstream file;
  file.open(calibrationDataFile.c_str(), ios::in);

  //assuming one value for each half-cm from 10cm to 80cm
  int i = 0;
  while (file >> IRDistanceMap[i][1]){
      IRDistanceMap[i][0] = 10 + 0.5*i;
      cout << this->IRDistanceMap[i][0] << "cm = " << this->IRDistanceMap[i][1] << endl;
      i++;
  }
  file.close();
}

double IRsensor::getValue() {
  value = this->adc.getAverage();
  return value;
}

double IRSensor::getDistance() {
  double distance;
  
  for (int i = 0; i < 142; i++){
    if (ADCValue > this->IRDistanceMap[i][1]){
      return (this->IRDistanceMap[i-1][0] + this->IRDistanceMap[i][0])/2;
    }
  }
  return 0;
}


