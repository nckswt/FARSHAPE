#include "Helm.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <pthread.h>

#include <sstream>

#define MOTOR_SPEED 80
#define MOTOR_PAUSE 200000

pthread_t updater;

Helm* tom(false);

void* callHandle(void *data) {
  
  tom->publish();
}

Helm::Helm( bool t ) {
  
  if (t) {
  
  leftEncoder  = Encoder ( ENCODER1_ADDRESS, false, false );
  rightEncoder = Encoder ( ENCODER2_ADDRESS, true,  true  );
  
  leftMotor  = Motor ( LEFT_MOTOR_PIN,  LEFT_MOTOR_REVERSED,  "/dev/servoblaster" );
  rightMotor = Motor ( RIGHT_MOTOR_PIN, RIGHT_MOTOR_REVERSED, "/dev/servoblaster" );
  
  currentPos.x = 0;
  currentPos.y = 0;
  currentPos.z = 0;
  
  tom = this;
  
//   pthread_create( &updater, NULL, this->publish, NULL );
  char* msg1 = "hello";
  pthread_create( &updater, NULL, &callHandle, (void*) msg1 );
  }
}

void Helm::resetEncoders() {
  
  this->leftEncoder .resetPosition();
  this->rightEncoder.resetPosition();
  
}

/** Go a relative distance and update the current location
 * 
 * @TODO fix code so that it constantly updates position. Currently it only 
 * 	 updates the location once the final distance has been travelled. This
 *       could be problematic if the system is interrupted, because it won't
 *       update the current position
 * @TODO fix glitchiness of motor encoders
 * @TODO make the code smarter? maybe we can set some vars or something to make
 * 	 the code structure a bit less complex.
 * 
 * @PARAM distance the distance to travel in cm
 * */
void Helm::goDistance ( float distance ) {
  
  resetEncoders();
  
  if (distance > 0) {

    // full ahead
    leftMotor .setSpeed(MOTOR_SPEED);
    rightMotor.setSpeed(MOTOR_SPEED);

    // glitch compensator
    usleep(MOTOR_PAUSE);

    // drive until each encoder has reached its distance
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( leftEncoder .getPosition() ) >= distance ) && (leftMotor .getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( rightEncoder.getPosition() ) >= distance ) && (rightMotor.getSpeed() != 0) )
	rightMotor.setSpeed(0);
      
    }
  
  }

  if (distance < 0) {
    // full back
    leftMotor .setSpeed(-MOTOR_SPEED);
    rightMotor.setSpeed(-MOTOR_SPEED);

    // glitch compensator
    usleep(MOTOR_PAUSE);

    // drive until each encoder has reached its distance
    while ( (leftMotor.getSpeed() != 0) || (rightMotor.getSpeed() != 0) ) {
    
      if ( ( convertToCm( MAX_VALUE - leftEncoder .getPosition() ) >= abs(distance) ) && (leftMotor .getSpeed() != 0) )
	leftMotor .setSpeed(0);
      
      if ( ( convertToCm( MAX_VALUE - rightEncoder.getPosition() ) >= abs(distance) ) && (rightMotor.getSpeed() != 0) )
	rightMotor.setSpeed(0);
    
    }
  }
  // update current location 
  //updateXY( distance );
  // glitch compensator
  usleep(MOTOR_PAUSE);
}

/** Helper function to convert degrees to arc length distance
 * 
 * @PARAM theta angle to convert in degrees
 * */
float Helm::degToArcLength( float theta ) {
  
  return theta*(M_PI/180)*17.145;
  
}

/** Rotates a specific relative angle and updates current rotation
 * 
 * @TODO fix code so that it constantly updates rotation. Currently it only 
 * 	 updates the rotation once it has rotated through the entire angle. This
 *       could be problematic if the system is interrupted, because it won't
 *       update the current rotation
 * @TODO fix glitchiness of motor encoders
 * @TODO make the code smarter? maybe we can set some vars or something to make
 * 	 the code structure a bit less complex.
 * */
void Helm::rotate( float theta ) {
  
  // reset encoders to get a fresh relative reading
  resetEncoders(); 
  
  // rotation is clockwise
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
  
  // rotation is counterclockwise
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
  // update the current rotation
  updateRotation( theta );
  // glitch compensator
  usleep(MOTOR_PAUSE);
}

void Helm::_readEncoders() {
}

void Helm::_setMotors() {
}

void Helm::updateRotation( float theta ) {
  
  this->rotation = this->rotation + theta;
  while (this->rotation < 0) {
    this->rotation = this->rotation + 360;
  }
  while (abs(this->rotation) > 360) {
    this->rotation = this->rotation - 360;
  }
  
}

void Helm::updateXY( float distance ) {
  
  this->currentPos.x = distance * sinf( this->rotation );
  this->currentPos.y = distance * cosf( this->rotation );
  
}

void Helm::publish() {
  char ** argv;
  int argc=1;
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::String>("posReporter", 1000);
  ros::Rate loop_rate(10);
  while (ros::ok()) {
    std_msgs::String msg;
    
    std::stringstream ss;
    ss << "Current position: X-" << currentPos.x <<
	  " Y-" << currentPos.y << " Z-" << currentPos.z;
    msg.data = ss.str();
    ROS_INFO("%s", msg.data.c_str());
    
    chatter_pub.publish(msg);
    
    loop_rate.sleep();
    
  }
}

void Helm::goTo( xyz location ) {
  
  xyz delta;
  float distance;
  float theta;
  
  delta.x = location.x - currentPos.x;
  delta.y = location.y - currentPos.y;
  delta.z = location.z - currentPos.z;
  
  distance = sqrt( pow(delta.x, 2) + pow(delta.y, 2) );
  theta = atanf( delta.x / delta.y ) - this->rotation;
  
  rotate( theta );
  goDistance( distance );
  
}
  
xyz Helm::getLocation() {
  return currentPos; 
}

float Helm::getRotation() {
  return this->rotation;
}

void Helm::goForward( int speed = MOTOR_SPEED ) {
  leftMotor .setSpeed( speed );
  rightMotor.setSpeed( speed );
}

void Helm::rotateLeft( int speed = MOTOR_SPEED ) {
  leftMotor .setSpeed( -speed );
  rightMotor.setSpeed(  speed );
}

void Helm::rotateRight( int speed = MOTOR_SPEED ) {
  leftMotor .setSpeed(  speed );
  rightMotor.setSpeed( -speed );
}

void Helm::stop() {
  leftMotor .setSpeed( 0 );
  rightMotor.setSpeed( 0 );
}
  
Helm::~Helm() {

  pthread_join( updater, NULL );
  
}
