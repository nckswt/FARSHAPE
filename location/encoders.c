#include "../system/i2c.h"
#include <unistd.h>
#include <stdint.h>

#define ENCODER1_ADDRESS 0x20
#define ENCODER2_ADDRESS 0x22
#define ENCODER_DEFAULT_ADDRESS 0x30

#define DISABLE_TERMINATOR 0x4B
#define ENABLE_TERMINATOR 0x4C
#define CHANGE_DEVICE_ADDRESS 0x4D

// maybe these should be declared in an array / iterator? 
#define ROTATION_0 0x41
#define ROTATION_1 0x40
#define ROTATION_2 0x43
#define ROTATION_3 0x42
#define ROTATION_BYTES 4

#define SIGNED_VELOCITY_0 0x3E
#define SIGNED_VELOCITY_1 0x3F
#define SIGNED_VELOCITY_BYTES 2

int getEncoderPosition( int encoderAddress ) {
  
  uint32_t position;
  position = 0xffffffff;
  position = i2c_read ( encoderAddress, ROTATION_0 );
  position = (position << 8) | i2c_read( encoderAddress, ROTATION_1 );
  position = (position << 8) | i2c_read( encoderAddress, ROTATION_2 );
  position = (position << 8) | i2c_read( encoderAddress, ROTATION_3 );

}

void setupEncoders() {

  // Change the first encoder's address to 0x20
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, ENCODER1_ADDRESS );
  
  // disable the terminator on the first encoder so we can talk to the second
  // encoder
  i2c_write( ENCODER1_ADDRESS, 	DISABLE_TERMINATOR, 0xff );
  
  // change the second encoder's address to 0x22
  i2c_write( ENCODER_DEFAULT_ADDRESS, CHANGE_DEVICE_ADDRESS, ENCODER2_ADDRESS );
  
}

