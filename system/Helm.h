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
  
  float rotation; // rotation in degrees from y axis. 
  
  xyz currentPos;
  robot robots[3];
  part* parts;
  
  void resetEncoders();
  float degToArcLength( float theta );
  void updateRotation( float theta );
  void updateXY( float distance );
  void _readEncoders(); // unused for now
  void _setMotors(); // unused for now
  
  
public:
  void publish();
  void goTo( xyz location ); // unused for now
  void goDistance( float distance );
  void rotate( float theta );
  void goForward( int speed );
  void rotateLeft( int speed );
  void rotateRight( int speed );
  void stop();
  Helm( bool t = true );
  ~Helm();
  xyz getLocation();
  float getRotation();
  
};
#endif