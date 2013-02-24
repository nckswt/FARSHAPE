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
#define ROTATION_4 0x44
#define ROTATION_5 0x45
#define ROTATION_BYTES 4

#define SIGNED_VELOCITY_0 0x3E
#define SIGNED_VELOCITY_1 0x3F
#define SIGNED_VELOCITY_BYTES 2

uint8_t* getEncoderPosition( int encoderAddress ) {
  
  uint8_t position[6];
  position[0] = 0x44;
  position[1] = 0x44;
  position[2] = 0x44;
  position[3] = 0x44;
  position[4] = 0x44;
  position[5] = 0x44;
//   position = position & ( i2c_read( encoderAddress, ROTATION_0 ) << 0 );
//   position = position & ( i2c_read( encoderAddress, ROTATION_1 ) << 8 );
//   position = position & ( i2c_read( encoderAddress, ROTATION_2 ) << 16);
//   position = position & ( i2c_read( encoderAddress, ROTATION_3 ) << 24);
//   position = position & ( i2c_read( encoderAddress, ROTATION_4 ) << 32);
//   position = position & ( i2c_read( encoderAddress, ROTATION_5 ) << 40);

  return position;
  
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

