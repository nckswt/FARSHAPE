#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <stdint.h>

#define DEFAULTSPEED 30

#define LEFT_MOTOR_PIN 		2
#define RIGHT_MOTOR_PIN		1

#define LEFT_MOTOR_REVERSED	true
#define RIGHT_MOTOR_REVERSED	false

class Motor {
private:
  uint8_t pin;
  uint8_t currentSpeed;
  bool reversed; 
  char* ctrl_interface;
  void _setValue( signed int speed );
public:
  void setSpeed( int speed );
  int getSpeed();
  Motor( uint8_t pin, bool reversed, char* ctrl_interface );
  Motor();
  ~Motor();
};

#endif