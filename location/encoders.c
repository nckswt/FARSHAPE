#include "../system/i2c.h"
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include "encoders.h"

Encoder::Encoder( int chipAddress, bool finalEncoder, int bus ) {
  this.chipAddress = chipAddress;
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, chipAddress << 1 );
  if (!finalEncoder)
    i2c_write_probe( ENCODER1_ADDRESS, 	DISABLE_TERMINATOR);
  this.bus = bus;
}

uint64_t Encoder::getEncoderPosition() {
  
  int position[6];
  position[0] = i2c_read( this.chipAddress, ROTATION_0 );
  position[1] = i2c_read( this.chipAddress, ROTATION_1 );
  position[2] = i2c_read( this.chipAddress, ROTATION_2 );
  position[3] = i2c_read( this.chipAddress, ROTATION_3 );
  position[4] = i2c_read( this.chipAddress, ROTATION_4 );
  position[5] = i2c_read( this.chipAddress, ROTATION_5 );
  
  uint64_t p = 0;
  
  for ( int i=5; i >= 0; i-- ) {
    p = (p << 8) + position[i];
//     std::cout << "Byte " << i << " is " << position[i] << std::endl;
  }
  
  return p;

}

uint64_t getEncoderPosition( int encoderAddress ) {
  
  int position[6];
  position[0] = i2c_read( encoderAddress, ROTATION_0 );
  position[1] = i2c_read( encoderAddress, ROTATION_1 );
  position[2] = i2c_read( encoderAddress, ROTATION_2 );
  position[3] = i2c_read( encoderAddress, ROTATION_3 );
  position[4] = i2c_read( encoderAddress, ROTATION_4 );
  position[5] = i2c_read( encoderAddress, ROTATION_5 );
  
  uint64_t p = 0;
  
  for ( int i=5; i >= 0; i-- ) {
    p = (p << 8) + position[i];
//     std::cout << "Byte " << i << " is " << position[i] << std::endl;
  }
  
  return p;
  
}

/*
int * getEncoderPosition( int encoderAddress ) {
  int p = i2c_read( encoderAddress, ROTATION_0 );
  std::cout << p << std::endl;
  int* pp = &pi;
  return pp;
}*/

void setupEncoders() {

  // Change the first encoder's address to 0x20
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, ENCODER1_ADDRESS << 1 );
  
  // disable the terminator on the first encoder so we can talk to the second
  // encoder
  i2c_write_probe( ENCODER1_ADDRESS, 	DISABLE_TERMINATOR);
  
  // change the second encoder's address to 0x22
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, ENCODER2_ADDRESS << 1 );
  
}

