#include "Helm.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
// #include "ros/ros.h"
// #include "std_msgs/String.h"
// #include <pthread.h>

#include <sstream>

#define MOTOR_SPEED 80
#define MOTOR_PAUSE 200000
#define LEFT_ADC_CHANNEL 0 //<>CHECK
#define RIGHT_ADC_CHANNEL 1 //<>CHECK


// pthread_t updater;

Helm::Helm() {
  
  left_encoder = Encoder ( ENCODER1_ADDRESS, false, false );
  right_encoder = Encoder ( ENCODER2_ADDRESS, true,  true );

  left_IR = IRsensor(LEFT_ADC_CHANNEL);
  right_IR = IRsensor(RIGHT_ADC_CHANNEL);

  left_motor = Motor ( LEFT_MOTOR_PIN,  LEFT_MOTOR_REVERSED,  "/dev/servoblaster" );
  right_motor = Motor ( RIGHT_MOTOR_PIN, RIGHT_MOTOR_REVERSED, "/dev/servoblaster" );
  
  current_position.x = 0;
  current_position.y = 0;
  current_position.z = 0;
  
//   pthread_create( &updater, NULL, this->publish, NULL );
  
}

void Helm::resetEncoders() {
  
  this->left_encoder.resetPosition();
  this->right_encoder.resetPosition();
  
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
    left_motor.setSpeed(MOTOR_SPEED);
    right_motor.setSpeed(MOTOR_SPEED);

    // glitch compensator
    usleep(MOTOR_PAUSE);

    // drive until each encoder has reached its distance
    while ( (left_motor.getSpeed() != 0) || (right_motor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( left_encoder.getPosition() ) >= distance ) && (left_motor.getSpeed() != 0) )
	left_motor.setSpeed(0);
      
      if ( ( convertToCm( right_encoder.getPosition() ) >= distance ) && (right_motor.getSpeed() != 0) )
	right_motor.setSpeed(0);
      
    }
  
  }

  if (distance < 0) {
    // full back
    left_motor.setSpeed(-MOTOR_SPEED);
    right_motor.setSpeed(-MOTOR_SPEED);

    // glitch compensator
    usleep(MOTOR_PAUSE);

    // drive until each encoder has reached its distance
    while ( (left_motor.getSpeed() != 0) || (right_motor.getSpeed() != 0) ) {
    
      if ( ( convertToCm( MAX_VALUE - left_encoder.getPosition() ) >= abs(distance) ) && (left_motor.getSpeed() != 0) )
	left_motor.setSpeed(0);
      
      if ( ( convertToCm( MAX_VALUE - right_encoder.getPosition() ) >= abs(distance) ) && (right_motor.getSpeed() != 0) )
	right_motor.setSpeed(0);
    
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
    
    
    left_motor.setSpeed(-MOTOR_SPEED);
    right_motor.setSpeed( MOTOR_SPEED);
    usleep(MOTOR_PAUSE);
    float distance = degToArcLength( theta );
    while ( (left_motor.getSpeed() != 0) || (right_motor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( MAX_VALUE - left_encoder.getPosition() ) >= distance ) && (left_motor.getSpeed() != 0) )
	left_motor.setSpeed(0);
      
      if ( ( convertToCm( right_encoder.getPosition() ) >= distance ) && (right_motor.getSpeed() != 0) )
	
	right_motor.setSpeed(0);
    
    }
    
  }
  
  // rotation is counterclockwise
  if (theta < 0) {
    
    left_motor.setSpeed( MOTOR_SPEED);
    right_motor.setSpeed(-MOTOR_SPEED);
    usleep(MOTOR_PAUSE);
    float distance = degToArcLength( theta );
    while ( (left_motor.getSpeed() != 0) || (right_motor.getSpeed() != 0) ) {
      
      if ( ( convertToCm( left_encoder.getPosition() ) >= abs(distance) ) && (left_motor.getSpeed() != 0) )
	left_motor.setSpeed(0);
      
      if ( ( convertToCm( MAX_VALUE - right_encoder.getPosition() ) >= abs(distance) ) && (right_motor.getSpeed() != 0) )
	right_motor.setSpeed(0);
    
    }
    
  }
  // update the current rotation
  updateRotation( theta );
  // glitch compensator
  usleep(MOTOR_PAUSE);
}

float Helm::rightRange(){
  return right_IR.getDistance();
}

float Helm::leftRange(){
  return left_IR.getDistance();
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
  
  this->current_position.x = distance * sinf( this->rotation );
  this->current_position.y = distance * cosf( this->rotation );
  
}

// void* Helm::publish(void* ptr) {
//   
//   ros::init(NULL, NULL, "posReporter");
//   ros::NodeHandle n;
//   ros::Publisher chatter_pub = n.advertise<std_msgs::String>("posReporter", 1000);
//   ros::Rate loop_rate(10);
//   while (ros::ok()) {
//     std_msgs::String msg;
//     
//     std::stringstream ss;
//     ss << "Current position: X-" << current_position.x <<
// 	  " Y-" << current_position.y << " Z-" << current_position.z;
//     msg.data = ss.str();
//     ROS_INFO("%s", msg.data.c_str());
//     
//     chatter_pub.publish(msg);
//     
//     loop_rate.sleep();
//     
//   }
// }

void Helm::goTo( xyz location ) {
  
  xyz delta;
  float distance;
  float theta;
  
  delta.x = location.x - current_position.x;
  delta.y = location.y - current_position.y;
  delta.z = location.z - current_position.z;
  
  distance = sqrt( pow(delta.x, 2) + pow(delta.y, 2) );
  theta = atanf( delta.x / delta.y ) - this->rotation;
  
  rotate( theta );
  goDistance( distance );
  
}
  
xyz Helm::getLocation() {
  return current_position; 
}

float Helm::getRotation() {
  return this->rotation;
}

void Helm::goForward( int speed = MOTOR_SPEED ) {
  left_motor.setSpeed( speed );
  right_motor.setSpeed( speed );
}

void Helm::rotateLeft( int speed = MOTOR_SPEED ) {
  left_motor.setSpeed( -speed );
  right_motor.setSpeed(  speed );
}

void Helm::rotateRight( int speed = MOTOR_SPEED ) {
  left_motor.setSpeed(  speed );
  right_motor.setSpeed( -speed );
}

void Helm::stop() {
  left_motor.setSpeed( 0 );
  right_motor.setSpeed( 0 );
}
  
Helm::~Helm() {

//   pthread_join( updater );
  
}
