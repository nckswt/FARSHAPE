#ifndef _ADC_SPI_H_
#define _ADC_SPI_H_

#define AVERAGING 100
#define START_BYTE 0b00000001
#define SINGLE 1
#define DIFFERENTIAL 0
#define SINGLE_DIFF_CONFIG 3

class ADC_SPI {
private:
  char adcChannel;
  char configBits;
  char spiChannel;
  char RWBuffer[3];
  
public:
  ADC_SPI( int adcChannel, int spiChannel );
  ~ADC_SPI();
  int getSingle();
  double getAverage();
};

#endif