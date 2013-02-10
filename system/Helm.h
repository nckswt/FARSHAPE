#ifndef _HELM_H_
#define _HELM_H_

#include <stdint.h>
#include "../location/Encoder.h"
#include "../mobility/Motor.h"

struct xyz {
  float x;
  float y;
  float z;
};

struct robot {
  int id; // is an enum
  xyz location;
  int status; // is an enum
};

struct part {
  int type; // is an enum
  xyz location;
  part* next;
};

class Helm {
  
private:
  
  Encoder leftEncoder;
  Encoder rightEncoder;
  Motor leftMotor;
  Motor rightMotor;
  xyz currentPos;
  robot robots[3];
  part* parts;
  
  void resetEncoders();
  float degToArcLength( float theta );
  
  void _readEncoders(); // unused for now
  void _setMotors(); // unused for now
  
public:
  
  void goTo( xyz location ); // unused for now
  void goDistance( float distance );
  void rotate( float theta );
  Helm();
  ~Helm();
  
};
#endif