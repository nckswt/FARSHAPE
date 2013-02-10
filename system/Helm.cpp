#include "Helm.h"
#include <math.h>
#include <stdlib.h>

Helm::Helm() {
  
  leftEncoder  = Encoder ( ENCODER1_ADDRESS, false, false );
  rightEncoder = Encoder ( ENCODER2_ADDRESS, true,  true  );
  
  leftMotor  = Motor ( LEFT_MOTOR_PIN,  LEFT_MOTOR_REVERSED,  "/dev/servoblaster" );
  rightMotor = Motor ( RIGHT_MOTOR_PIN, RIGHT_MOTOR_REVERSED, "/dev/servoblaster" );
  
  currentPos.x = 0;
  currentPos.y = 0;
  currentPos.z = 0;
  
}

void Helm::resetEncoders() {
  
  this->leftEncoder .resetPosition();
  this->rightEncoder.resetPosition();
  
}

void Helm::goDistance ( float distance ) {
  
  resetEncoders();
  
  leftMotor .setSpeed(100);
  rightMotor.setSpeed(100);
  
  if (distance > 0) {
  
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( convertToCm( leftEncoder .getPosition() ) >= distance )
	leftMotor .setSpeed(0);
      
      if ( convertToCm( rightEncoder.getPosition() ) >= distance )
	rightMotor.setSpeed(0);
      
    }
  
  }

  if (distance < 0) {
    
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
    
      if ( MAX_VALUE - convertToCm( leftEncoder .getPosition() ) >= abs(distance) )
	leftMotor .setSpeed(0);
      
      if ( MAX_VALUE - convertToCm( rightEncoder.getPosition() ) >= abs(distance) )
	rightMotor.setSpeed(0);
    
    }
  
  }

}

float Helm::degToArcLength( float theta ) {
  
  return 2*M_PI*17.145*theta;
  
}

void Helm::rotate( float theta ) {
  
  resetEncoders(); 
  
  if (theta > 0) {
    
    leftMotor .setSpeed(-100);
    rightMotor.setSpeed( 100);
    
    float distance = degToArcLength( theta );
    
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( MAX_VALUE - convertToCm( leftEncoder.getPosition() ) >= distance )
	leftMotor .setSpeed(0);
      
      if ( convertToCm( rightEncoder.getPosition() ) >= distance )
	
	rightMotor.setSpeed(0);
    
    }
    
  }
  
  if (theta < 0) {
    
    leftMotor .setSpeed( 100);
    rightMotor.setSpeed(-100);
    
    float distance = degToArcLength( theta );
    
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( convertToCm( leftEncoder.getPosition() ) >= distance )
	leftMotor .setSpeed(0);
      
      if ( MAX_VALUE - convertToCm( rightEncoder.getPosition() ) >= distance )
	rightMotor.setSpeed(0);
    
    }
    
  }
  
}

void Helm::goTo( xyz location ) {
}

void Helm::_readEncoders() {
}

void Helm::_setMotors() {
}

Helm::~Helm() {
}