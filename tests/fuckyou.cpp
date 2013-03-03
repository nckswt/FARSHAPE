#include "../location/Encoder.h"
#include "../mobility/Motors.h"
#include <stdint.h>



int main() {
  
  Encoder leftEncoder ( ENCODER1_ADDRESS, false, false );
  Encoder rightEncoder( ENCODER2_ADDRESS, true,  true  ); 
  
  Motors motors;
  
  float distance = 5; // go 5 cm
  
  uint64_t leftPosInitial;
  uint64_t rightPosInitial;
  
  uint64_t leftPos;
  uint64_t rightPos; 
  
  motors.goForward();
  
  while (1) {
    if ((convertToCm(leftEncoder .getPosition()) > distance) && 
      (convertToCm(rightEncoder.getPosition()) > distance)) {
	  break;
    }
  }
  
  motors.brake();
  
  return 0;

  
}