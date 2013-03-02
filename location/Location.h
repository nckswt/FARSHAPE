#ifndef _LOCATION_H
#define _LOCATION_H
#include "Encoder.h"

class Location {
private:
  float x;
  float y;
  float xPos;
  float yPos;
  float rotation;
  Encoder leftEncoder;
  Encoder rightEncoder;
  uint64_t leftEncoderPos;
  uint64_t rightEncoderPos;
  void readFromEncoders();
public:
  Location ();
  ~Location ();
  void updateLocation();
  float convertToCm( int bits );
  float convertToBits ( float cm );
  int getX();
  int getY();
};
#endif