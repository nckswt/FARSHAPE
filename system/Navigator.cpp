#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/time.h>
#include "Navigator.h"
#include "../location/Encoder.h"
#define PI 3.14159265

// volatile Navigator nav;


/** Constructor for Navigator object
 * 
 * Sets up all stuff needed to constantly update the robot's own position
 * and to communicate that position to the rest of the robots. It sets up
 * a timer which continuously reads from the encoders updates its internal
 * position. A second, much slower timer is set up through ROS to update
 * the other robots on its current position and also receive the current
 * position from the other robots. 
 * */

Navigator::Navigator(){
  
  Encoder leftEncoder;
  Encoder rightEncoder;
  
  leftEncoder = Encoder ( ENCODER1_ADDRESS , false, false );
  rightEncoder= Encoder ( ENCODER2_ADDRESS , true, true );
  
  leftPos = 0;
  rightPos = 0;

  while (1) {
    _readEncoders();
    usleep(10000);
  }
  
}

void Navigator::_readEncoders() {
  
  this->leftPos  = leftEncoder .getPosition();
  this->rightPos = rightEncoder.getPosition();
  
//   int ret;
//   
//   ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(10) );
  
}

uint64_t Navigator::getLeft() {
  
  return leftPos;
  
}

Navigator::~Navigator(){

}

/*
void Navigator::goDistance(double targetDistance, int speed = 30, Location pos){
    int initialPosition;
    
    if (targetDistance > 0){
        motors.goForward();
    } else {
        motors.goBackward();
    }
    while()
    	currentPosition = encoders.getLinearPositionDelta();
    motors.brake();
}*/

/*void Navigator::identifyTarget(std::string imageFile){
	//open filename
	//get x, z coordinates
	//close filename

	//read IR sensor
	//get y coordinate

	//set self.targetPosition

	//<>TODO: REMOVE THIS STUB----------------------------
	srand(time(NULL));
	this->targetPosition.x = rand() % 7 + 3;
	this->targetPosition.y = rand() % 7 + 3;
	this->targetPosition.z = rand() % 3;
	//----------------------------------------------------
}*/

/*
SphCoord Navigator::cart2spherical(Cart3DCoord coord){
    SphCoord spherical;
    spherical.r = sqrt(pow(coord.x,2) + pow(coord.y,2) + pow(coord.z,2));
    spherical.theta = acos(coord.z/spherical.r) * 180.0 / PI; 
    spherical.phi = atan2(coord.y,coord.x) * 180.0 / PI;
    return spherical;
}

Cart3DCoord Navigator::getTargetPosition(){
    return this->targetPosition;
}

int runNav() {
  
}
*/