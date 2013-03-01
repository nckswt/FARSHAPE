#include "Encoder.h"

class Location {
private:
  signed float x;
  signed float y;
  signed float xPos;
  signed float yPos;
  signed float rotation;
  Encoder leftEncoder;
  Encoder rightEncoder;
  uint64_t leftEncoderPos;
  uint64_t rightEncoderPos;
  void readFromEncoders();
public:
  Location ();
  ~Location ();
  void updateLocation();
  int getX();
  int getY();
};