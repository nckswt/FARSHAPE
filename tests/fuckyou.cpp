#include "../location/Encoder.h"
#include "../mobility/Motors.h"
#include <stdint.h>
#include <iostream>
#include <pthread.h>

#define SLEEP_TIME 10000


  Encoder leftEncoder ( ENCODER1_ADDRESS, false, false );
  Encoder rightEncoder( ENCODER2_ADDRESS, true,  true  ); 

int main() {
  
  

  
  Motors motors;
  
  float distance = 60; // go 60 cm
  
  uint64_t leftPosInitial;
  uint64_t rightPosInitial;
  
  uint64_t leftPos;
  uint64_t rightPos; 
  
  motors.goForward(30);
  
  bool stop = false;
  
  while (!stop) {

    if ((convertToCm(leftEncoder .getPosition()) > distance) && 
      (convertToCm(rightEncoder.getPosition()) > distance)) {
	  motors.brake();
      std::cout << "Left encoder position in cm: " <<  convertToCm(leftEncoder .getPosition()) << std::endl;
	std::cout << "Right encoder position in cm: " << convertToCm(rightEncoder.getPosition()) << std::endl;
      stop = true;
    }
    usleep(SLEEP_TIME);
  }

  usleep(SLEEP_TIME);
  motors.idle();
  
  return 0;

  
}