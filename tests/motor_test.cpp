#include "../mobility/Motor.h"
#include <unistd.h>
#include <iostream>

int main() {
  
  std::cout << "Making left motor" << std::endl;
  Motor leftMotor ( LEFT_MOTOR_PIN,  LEFT_MOTOR_REVERSED,  "/dev/servoblaster" );
  std::cout << "Making right motor" << std::endl;
  Motor rightMotor( RIGHT_MOTOR_PIN, RIGHT_MOTOR_REVERSED, "/dev/servoblaster" );
  std::cout << "Made both motors" << std::endl;

  std::cout << "Left motor: 100% forward" << std::endl;
  leftMotor.setSpeed(100);
  sleep(3);
  std::cout << "Left motor: 0% forward" << std::endl;
  leftMotor.setSpeed(0);
  sleep(3);
  
  std::cout << "Right motor: 100% forward" << std::endl;
  rightMotor.setSpeed(100);
  sleep(3);
  std::cout << "Right motor: 0% forward" << std::endl;
  rightMotor.setSpeed(0);
  sleep(3);
  
  std::cout << "Left and Right motor: 100% forward" << std::endl;
  leftMotor.setSpeed(100);
  rightMotor.setSpeed(100);
  sleep(3);
  std::cout << "Left and Right motor: 0% forward" << std::endl;
  rightMotor.setSpeed(0);
  leftMotor.setSpeed(0);
  sleep(3);
  
    std::cout << "Left motor: 100% backward" << std::endl;
  leftMotor.setSpeed(-100);
  sleep(3);
  std::cout << "Left motor: 0% backward" << std::endl;
  leftMotor.setSpeed(0);
  sleep(3);
  
  std::cout << "Right motor: 100% backward" << std::endl;
  rightMotor.setSpeed(-100);
  sleep(3);
  std::cout << "Right motor: 0% backward" << std::endl;
  rightMotor.setSpeed(0);
  sleep(3);
  
  std::cout << "Left and Right motor: 100% backward" << std::endl;
  leftMotor.setSpeed(-100);
  rightMotor.setSpeed(-100);
  sleep(3);
  std::cout << "Left and Right motor: 0% backward" << std::endl;
  rightMotor.setSpeed(0);
  leftMotor.setSpeed(0);
  sleep(3);
  
  return 0;
  
}