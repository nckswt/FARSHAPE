#ifndef _ENCODER_H_
#define _ENCODER_H_
#include <unistd.h>
#include <stdint.h>

#define ENCODER1_ADDRESS 0x20
#define ENCODER2_ADDRESS 0x22
#define ENCODER_DEFAULT_ADDRESS 0x30

#define RESET_POSITION 0x4A
#define DISABLE_TERMINATOR 0x4B
#define ENABLE_TERMINATOR 0x4C
#define CHANGE_DEVICE_ADDRESS 0x4D

// maybe these should be declared in an array / iterator? 
#define ROTATION_0 0x41
#define ROTATION_1 0x40
#define ROTATION_2 0x43
#define ROTATION_3 0x42
#define ROTATION_4 0x47
#define ROTATION_5 0x46
#define ROTATION_BYTES 6

#define SIGNED_VELOCITY_0 0x3E
#define SIGNED_VELOCITY_1 0x3F
#define SIGNED_VELOCITY_BYTES 2

#define MAX_VALUE 281474976710655


class Encoder {
private:
  
  // members
  uint8_t chipAddress;
  bool finalEncoder;
  uint8_t bus;
  bool reversed;
  
  // helper functions
  void _assert ( int flag );
  void _write ( int location, int data );
  uint8_t _read ( int location );
  
public:
  
  uint64_t getPosition();
  Encoder( int chipAddress , bool finalEncoder, bool reversed, int bus = 1 );
  Encoder();
  ~Encoder( );
  void resetPosition();
  int getAddress();
  
};

float convertToCm (uint64_t bits);

#endif