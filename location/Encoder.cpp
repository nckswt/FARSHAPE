#include "../system/i2c.h"
#include "Encoder.h"
#include <iostream>

#define BITS_TO_CM 0.050850058 // cm/bit
#define CM_TO_BITS 9.82499806173 // bits/cm 

Encoder::Encoder() { 
  
}

Encoder::Encoder( int chipAddress, bool finalEncoder, bool reversed, int bus ) {
  this->chipAddress = chipAddress;
  this->reversed = reversed;
  this->bus = bus;
  //
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, chipAddress << 1 );
  if (!finalEncoder)
    i2c_write_probe( this->chipAddress, DISABLE_TERMINATOR);
  i2c_write_probe( this->chipAddress, 0x4a);
}

uint64_t Encoder::getPosition() {
  
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
  
  // TODO: smarter thing to do here?
  if (this->reversed && p!=0)
    p=281474976710655-p;
  
  return p;

}

int Encoder::getAddress() {
  return this->chipAddress;
}

Encoder::~Encoder() {
  i2c_write_probe( this->chipAddress , 0x4a );
}

float convertToCm( uint64_t bits ) {
  return float( bits*BITS_TO_CM );
}