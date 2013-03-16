/*NOTE: This code uses the SPI-based LTC1098 ADC. 
The final robots will not use this ADC, 
so this is merely a test stub. */

#include "IRsensor.h"

/**
IR Sensor Constructor
*/
IRsensor::IRsensor(int adcChannel){
  this->numberOfSamples = 100;
  this->csPin = 5;
  this->adcChannel = adcChannel;
  this->calibrationDataFile = "calibration.txt";
  this->maxRange = 50.0; //cm
  this->IRoffset = 10.0; //cm

  //set LTC1098 channel
  if (adcChannel == 0)
    this->chanBits = 0b11010000;
  else
    this->chanBits = 0b11110000;

  //setup SPI
  wiringPiSetup();
  if( wiringPiSPISetup(adcChannel, 500000) < 0)
    cout << "SPI Setup failed: " << endl;
  pinMode(csPin, OUTPUT);
  digitalWrite(csPin, HIGH);

  //Calibrate IR Sensor
  readGP2D12CalibrationProfile();
}

/**
IR Sensor Destructor
*/
IRsensor::~IRsensor(){

}

/**
Read Calibration file to convert ADC values to distances
*/
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

/**
Read ADC values from the GP2D12
*/
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
    //cout << int(((spiData [0] << 7) | (spiData [1] >> 1)) & 0x3FF) << endl;
    averageADCValue += int(((spiData [0] << 7) | (spiData [1] >> 1)) & 0x3FF);
    digitalWrite(csPin,HIGH);

    usleep(10);
  }
  averageADCValue = averageADCValue/numberOfSamples;

  //cout << "AVADC: " << averageADCValue << endl;
  return averageADCValue;
}


/**
Generate the calibration file
*/
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

/**
Read ADC values, compare them to distances, and output distances
*/
double IRsensor::readGP2D12Distance(){
  double distance, ADCValue;
  ADCValue = readGP2D12ADC();
  //cout << "ADC VALUES: " << ADCValue << endl;  
  
  for (int i = 0; i < 142; i++){
    //cout << this->IRDistanceMap[i][1] << endl;
    if (ADCValue > this->IRDistanceMap[i][1]){
      //cout << ADCValue << " v " << IRDistanceMap[i][1] << endl;
      distance = (this->IRDistanceMap[i-1][0] + this->IRDistanceMap[i][0])/2;
      break;
    }
  }
  return distance;
}

double IRsensor::distanceFromRobot(){
  double distance; 
  distance = readGP2D12Distance();
  if (distance < this->IRoffset || distance > this->maxRange){
    distance = -1;
  } else {
    distance -= this->IRoffset;
  }
    return distance;
}