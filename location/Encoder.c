#include "../system/i2c.h"
#include "Encoder.h"
#include <iostream>

Encoder::Encoder() { 
  
}

Encoder::Encoder( int chipAddress, bool finalEncoder, bool reversed, int bus ) {
  this->chipAddress = chipAddress;
  this->reversed = reversed;
  //
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, chipAddress << 1 );
  if (!finalEncoder)
    i2c_write_probe( ENCODER1_ADDRESS, 	DISABLE_TERMINATOR);
  this->bus = bus;
}

uint64_t Encoder::getEncoderPosition() {
  
  int position[6];
  position[0] = i2c_read( this->chipAddress, ROTATION_0 );
  position[1] = i2c_read( this->chipAddress, ROTATION_1 );
  position[2] = i2c_read( this->chipAddress, ROTATION_2 );
  position[3] = i2c_read( this->chipAddress, ROTATION_3 );
  position[4] = i2c_read( this->chipAddress, ROTATION_4 );
  position[5] = i2c_read( this->chipAddress, ROTATION_5 );
  
  uint64_t p = 0;
  
  for ( int i=5; i >= 0; i-- ) {
    p = (p << 8) + position[i];
  }
  
  if (this->reversed)
    p=281474976710655-p;
  
  return p;

}

int Encoder::getAddress() {
  return this->chipAddress;
}

Encoder::~Encoder() {
}