/*NOTE: This code uses the SPI-based LTC1098 ADC. 
The final robots will not use this ADC, 
so this is merely a test stub. */

#include "IRsensor.h"

IRsensor::IRsensor(int adcChannel, string calibrationDataFile = "calibration.txt" ){
  this.numberOfSamples = 100;
  this.csPin = 5;
  this.adcChannel = adcChannel;
  this.calibrationDataFile = calibrationDataFile;

  //set LTC1098 channel
  if (adcChannel == 0)
    this.chanBits = 0b11000000;
  else
    this.chanBits = 0b11100000;

  //setup SPI
  wiringPiSetup();
  if( wiringPiSPISetup(adcChannel, 500000) < 0)
    cout << "SPI Setup failed: " << endl;
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  //Calibrate IR Sensor
  readGP2D12CalibrationProfile();
}

IRsensor::~IRsensor(){

}


void IRsensor::readGP2D12CalibrationProfile(){
  //open calibration profile file
  ifstream file;
  file.open(calibrationDataFile, ios::in);

  //assuming one value for each half-cm from 10cm to 80cm
  int i = 0;
  while (file >> IRDistanceMap[i][1]){
      IRDistanceMap[i][0] = 10 + 0.5*i;
      cout << IRDistanceMap[i][0] << "cm = " << IRDistanceMap[i][1] << endl;
      i++;
  }
  file.close();
}

double IRsensor::readGP2D12ADC(){
  unsigned char spiData[2];
  int bufferLength = 2;
  double averageADCValue = 0;

  //take numberOfSamples samples and find average value
  for (int i = 0; i < numberOfSamples; i++){
    spiData[0] = chanBits;
    spiData[1] = 0b0000000;

    digitalWrite(csPin,LOW);
    wiringPiSPIDataRW (adcChannel, spiData, bufferLength);
    averageADCValue += int(((spiData [0] << 7) | (spiData [1] >> 1)) & 0x3FF);
    digitalWrite(csPin,HIGH);

    usleep(10);
  }
  averageADCValue = averageADCValue/numberOfSamples;

  return averageADCValue;
}

void IRsensor::calibrateSensor(){
  ofstream file;
  file.open("sensorCalibrationData");

  for (int i = 0; i < 142; i++){
    cout<< "Sample " << i <<": ";
    file << i << '\t';
    file << readGP2D12ADC() << endl;
    sleep(5);
    }
 file.close();
}

double IRsensor::readGP2D12Distance(double IRDistanceMap[141][2]){
  double distance, ADCValue;
  ADCValue = readGP2D12ADC();
  cout << ADCValue;  
  for (int i = 0; i < 142; i++){
    if (ADCValue < IRDistanceMap[i][1]){
      distance = (IRDistanceMap[i-1][1] + IRDistanceMap[i][1])/2;
      break;
    }
  }
  return distance;
}


int main(int argc, char ** argv){
  IRsensor leftIR(0);
  IRsensor rightIR(1);

  //output distance
  while (1){
    cout << "Left distance: " << leftIR.readGP2D12Distance(IRDistanceMap) << "cm" << endl; 
    cout << "Right distance: " << rightIR.readGP2D12Distance(IRDistanceMap) << "cm" << endl; 
    sleep(1);
  }

  return 0;
}