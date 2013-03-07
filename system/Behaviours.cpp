#include "Behaviours.h"


Behaviours::Behaviours(){
}

Behaviours::~Behaviours(){

}

//Atomic Behaviours
void Behaviours::setupActuatorsAndSensors(){
	
}

//<>TODO: GET DISTANCE FROM ENCODERS
//move a set distance (positive for forward, negative for backward)
void Behaviours::goDistance(double targetDistance, int speed = 30, Location pos){
    int initialPosition;
    
    if (targetDistance > 0){
        motors.goForward();
    } else {
        motors.goBackward();
    }
    while()
    	currentPosition = encoders.getLinearPositionDelta();
    motors.brake();
}


//<>TODO: get angle data from encoders
//rotate a set angle (positive for counterclockwise, right for clockwise)
void Behaviours::rotateAngle(double targetAngle, int speed = 30){
    double initialAngle, currentAngle;
    initialAngle = currentAngle = 0;
    encoders.setAngularPosition();
    if (targetAngle > 0){
        motors.rotateLeft();
    } else {
        motors.rotateRight();
    }
    while(abs(currentAngle - initialAngle) < abs(targetAngle))
    	currentAngle = encoders.getAngularPositionDelta();
    motors.brake();
}

Cart3DCoord Behaviours::findTarget(){
	navigator.identifyTarget("testImageFile");
	return navigator.getTargetPosition();
}

void Behaviours::moveToTarget(SphCoord targetPosition){
	int bufferzone = 5; //cm in front of target

	this->rotateAngle(targetPosition.theta);
	this->goDistance(targetPosition.r - bufferzone);
}

void Behaviours::grabTarget(Cart3DCoord targetPosition){
	//arm.openGripper();
	//arm.moveToTarget();
	//arm.closeGripper(target.width);
}

void Behaviours::trackTarget(){
	Cart3DCoord targetCartPosition = this->findTarget();
	SphCoord targetPosition = navigator.cart2spherical(targetCartPosition);
	this->moveToTarget(targetPosition);
	this->grabTarget(targetCartPosition);
}