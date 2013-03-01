#include "Location.h"

// wheel diameter is 4" or 10.16cm
// #define WHEEL_DIAMETER 10.16
// #define WHEEL_RADIUS 5.08
// #define WHEEL_CIRCUMFERENCE 31.9185813605
// 313.6 BITS/REV / 31.9185813605 CM/REV = 9.82499806173 BITS/CM
// 1 / (9.82499806173 BITS/CM) = 0.10178119056 CM/BIT
#define CONVERSION_FACTOR 0.10178119056 // cm/bit

Location::Location () {
  x = 0;
  y = 0;
  rotation = 0;
  leftEncoder  = Encoder ( ENCODER1_ADDRESS, false );
  rightEncoder = Encoder ( ENCODER2_ADDRESS, true  ); 
}

Location::~Location() {
}

void Location::readFromEncoders() {
  leftEncoderPos  = leftEncoder ->getEncoderPosition();
  rightEncoderPos = rightEncoder->getEncoderPosition();
}

float Location::convertToCm( int bits ) {
  return bits*CONVERSION_FACTOR;
}

void Location::updateLocation() {
  readFromEncoders();
  xPos = ceil(leftEncoderPos,rightEncoderPos);
}
  
// 313.6 BITS/REV / 31.9185813605 CM/REV = 9.82499806173 BITS/CM
// 1 / (9.82499806173 BITS/CM) = 0.10178119056 CM/BIT