#ifndef _IRSENSOR_H_
#define _IRSENSOR_H_

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <map>
#include <fcntl.h>
#include <linux/kd.h>
#include <fstream>

using namespace std; // GET RID OF THIS!

class IRsensor
{
public:
	IRsensor();
	~IRsensor();
	double readGP2D12Distance();

	/* data */
    unsigned char chanBits;
	double IRDistanceMap[141][2];
	int numberOfSamples;
    int csPin;
    int adcChannel;
	string calibrationDataFile;

private:
	void readGP2D12CalibrationProfile();
	double readGP2D12ADC();
	void calibrateSensor();
};