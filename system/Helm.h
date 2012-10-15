#ifndef _HELM_H_
#define _HELM_H_

#include <stdint.h>
#include "../location/Encoder.h"
#include "../mobility/Motor.h"
#include "../location/IRsensor.h"
#include "includes.h"

struct robot {
  int id; // is an enum
  Position location;
  int status; // is an enum
};

struct part {
  int type; // is an enum
  Position location;
  part* next;
};

class Helm {
  
private:
  Encoder left_encoder;
  Encoder right_encoder;
  Motor left_motor;
  Motor right_motor;
  IRsensor left_IR;
  IRsensor right_IR;
  
  float rotation; // rotation in degrees from y axis. 
  
  Position current_position;
  robot robots[3];
  part* parts;
  
  void resetEncoders();
  float degToArcLength( float theta );
  void updateRotation( float theta );
  void updateXY( float distance );
//   void* publish(void* ptr);
  void _readEncoders(); // unused for now
  void _setMotors(); // unused for now
  
  
public:
  void goTo( Position location ); // unused for now
  void goDistance( float distance ); //in cm
  void rotate( float theta ); //in degrees
  void goForward( int speed );
  void rotateLeft( int speed );
  void rotateRight( int speed );
  float rightRange();
  float leftRange();
  void stop();
  Helm();
  ~Helm();
  Position getLocation();
  float getRotation();
  void lineUp();
  
};
#endif