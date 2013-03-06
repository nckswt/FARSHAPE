#include "Helm.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

#define MOTOR_SPEED 80
#define MOTOR_PAUSE 200000

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
  
  if (distance > 0) {

  leftMotor .setSpeed(MOTOR_SPEED);
  rightMotor.setSpeed(MOTOR_SPEED);

  usleep(MOTOR_PAUSE);

    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( leftEncoder .getPosition() ) >= distance ) && (leftMotor .getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( rightEncoder.getPosition() ) >= distance ) && (rightMotor.getSpeed() != 0) )
	rightMotor.setSpeed(0);
      
    }
  
  }

  if (distance < 0) {

    leftMotor .setSpeed(-MOTOR_SPEED);
    rightMotor.setSpeed(-MOTOR_SPEED);

    usleep(MOTOR_PAUSE);

    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
    
      if ( ( convertToCm( MAX_VALUE - leftEncoder .getPosition() ) >= abs(distance) ) && (leftMotor .getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( MAX_VALUE - rightEncoder.getPosition() ) >= abs(distance) ) && (rightMotor.getSpeed() != 0) )
	rightMotor.setSpeed(0);
    
    }
  
  }
  usleep(MOTOR_PAUSE);
}

float Helm::degToArcLength( float theta ) {
  
  return theta*(M_PI/180)*17.145;
  
}

void Helm::rotate( float theta ) {
  
  resetEncoders(); 
  
  if (theta > 0) {
    
    leftMotor .setSpeed(-MOTOR_SPEED);
    rightMotor.setSpeed( MOTOR_SPEED);
    usleep(MOTOR_PAUSE);
    float distance = degToArcLength( theta );
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( MAX_VALUE - leftEncoder.getPosition() ) >= distance ) && (leftMotor.getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( rightEncoder.getPosition() ) >= distance ) && (rightMotor.getSpeed() != 0) )
	
	rightMotor.setSpeed(0);
    
    }
    
  }
  
  if (theta < 0) {
    
    leftMotor .setSpeed( MOTOR_SPEED);
    rightMotor.setSpeed(-MOTOR_SPEED);
    usleep(MOTOR_PAUSE);
    float distance = degToArcLength( theta );
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( leftEncoder.getPosition() ) >= abs(distance) ) && (leftMotor.getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( MAX_VALUE - rightEncoder.getPosition() ) >= abs(distance) ) && (rightMotor.getSpeed() != 0) )
	rightMotor.setSpeed(0);
    
    }
    
  }
  usleep(MOTOR_PAUSE);
}

void Helm::goTo( xyz location ) {
}

void Helm::_readEncoders() {
}

void Helm::_setMotors() {
}

Helm::~Helm() {
}
