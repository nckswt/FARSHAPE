#include "../location/Motor.h"

int main() {
  
  Motor leftMotor ( LEFT_MOTOR_PIN,  LEFT_MOTOR_REVERSED,  "/dev/servoblaster" );
  Motor rightMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_REVERSED, "/dev/servoblaster" );
  
  leftMotor.setSpeed(100);
  sleep(3);
  leftMotor.setSpeed(0);
  sleep(3);
  
  rightMotor.setSpeed(100);
  sleep(3);
  rightMotor.setSpeed(0);
  sleep(3);
  
  leftMotor.setSpeed(100);
  rightMotor.setSpeed(100);
  sleep(3);
  rightMotor.setSpeed(0);
  leftMotor.setSpeed(0);
  sleep(3);
  
  return 0;
  
}