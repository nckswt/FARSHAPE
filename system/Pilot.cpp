#include "Pilot.h"
#include "../location/Encoder.h"
// #include "Behaviour.h"


Pilot::Pilot(){ 

  const char* c = "/dev/servoblaster";
  Motor leftMotor(2, true, "/dev/servoblaster");
  Motor rightMotor(1, false, "/dev/servoblaster");
  
}

void Pilot::moveTo ( float x, float y ) {
  // not yet implemented
}

/** Rotates the robot to a specific theta relative to absolute x-y
 * 
 * @param theta (Absolute) Rotation angle in degrees
 * */
void Pilot::rotate( int theta ) {
  // not yet implemented
}

/** Sets the navigator 
 * 
 * @param n The navigator reference to pass to the pilot
 * */
void Pilot::setNavigator( Navigator* n ) {
  
  this->n = n;

}
/** Helper function to go a specific distance
 * 
 * @param distance the distance to travel
 * */
void Pilot::_goDistance( float distance ) {
  
  _resetEncoders();
  
  this->leftMotor.setSpeed( 100 );
  this->rightMotor.setSpeed( 100 );
  
  bool exit = false;
  
  while (!exit) {
    if ( _getDistance(this->n->leftEncoder) < distance )
      exit = true;
  }
  
}

/** Reset encoder positions
 * @todo Implement error handling for when Navigator object is not set
 **/      
void Pilot::_resetEncoders() {
  if (this->n == NULL) {
    return;
  }

  this->n->leftEncoder.resetPosition();
  this->n->rightEncoder.resetPosition();

}

/** Gets encoder position and converts to distance
 * 
 * @param enc The encoder to pass
 * 
 * @return distance in cm
 * */
float Pilot::_getDistance( Encoder enc ) {
  return convertToCm( enc.getPosition() );
}

Pilot::~Pilot(){

}

/*void Pilot::executePlan(){
  this->releaseCondition = 1;
  while (checkReleaseCondition()){
    if (!this->planIsSetup)
      this->updatePlan();

    //<>TODO: REMOVE STUB!----------------------
    behaviours.trackTarget();
    //------------------------------------------
    // for(std::vector<Behaviours>::iterator it = plan.begin(); it != plan.end(); ++it) {
    // 	}
  }

}*/

/*void Pilot::updatePlan(){
	//set plan to track object

	//<>TODO: REMOVE STUB!----------------------
	behaviours.setupActuatorsAndSensors();
	//------------------------------------------
	this->planIsSetup = 1;
}*/

/*bool Pilot::checkReleaseCondition(){
	//<>TODO: REMOVE STUB!----------------------
	return 1;
	//------------------------------------------
}*/