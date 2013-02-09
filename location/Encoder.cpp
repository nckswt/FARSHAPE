#include "../system/i2c.h"
#include "Encoder.h"
#include <iostream>

#define BITS_TO_CM 0.050850058 // cm/bit
#define CM_TO_BITS 9.82499806173 // bits/cm 

Encoder::Encoder() { 
  
}

Encoder::Encoder( int chipAddress, bool finalEncoder, bool reversed, int bus ) {
  this->reversed = reversed;
  this->bus = bus;
  
  this->chipAddress = int(ENCODER_DEFAULT_ADDRESS); // to handle setup
  // Set encoder to new address. New address bits must be shifted
  _write( CHANGE_DEVICE_ADDRESS, chipAddress << 1 );
  
  // if successful, set new address
  this->chipAddress = chipAddress;
  
  // now that setup is done, disable terminator if we have other encoders
  if (!finalEncoder)
    _assert ( DISABLE_TERMINATOR );
  
  // reset the position to 0 before we use it
  resetPosition();
  
}

uint64_t Encoder::getPosition() {
  
  int position[6];
  position[0] = _read( ROTATION_0 );
  position[1] = _read( ROTATION_1 );
  position[2] = _read( ROTATION_2 );
  position[3] = _read( ROTATION_3 );
  position[4] = _read( ROTATION_4 );
  position[5] = _read( ROTATION_5 );
  
  uint64_t p = 0;
  
  for ( int i=5; i >= 0; i-- ) {
    p = (p << 8) + position[i];
  }
  
  // TODO: smarter thing to do here?
  if (this->reversed && p!=0)
    p=281474976710655-p;
  
  return p;

}

void Encoder::resetPosition() {
  _assert( RESET_POSITION ); 
}

int Encoder::getAddress() {
  return this->chipAddress;
}

Encoder::~Encoder() {
  resetPosition();
}



/** ************************************
 * Helper functions
 * TODO: handle errors
 *************************************** */
void Encoder::_assert(int flag) {
  i2c_write_probe( this->chipAddress , flag );
}

void Encoder::_write( int location, int data ) {
  
  i2c_write( this->chipAddress, location, data );
  
}

uint8_t Encoder::_read( int location ) {
  return i2c_read ( this->chipAddress, location );
}


float convertToCm( uint64_t bits ) {
  return float( bits*BITS_TO_CM );
}
