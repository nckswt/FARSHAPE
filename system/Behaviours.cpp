//#import "Actuators.h"
//#import "Sensors.h"
#import "Navigator.h"
#import "../mobility/Motors.h"

Behaviours::Behaviours(){
	trackObject =  {&setupActuatorsAndSensors,
					&findTarget,
					&moveToTarget,
					&grabTarget};
}

Behaviours::~Behaviours(){

}

//Atomic Behaviours
void Behaviours::setupActuatorsAndSensors(){
	Motors motors;
	Navigator navigator;
}

void Behaviours::findTarget(){
	navigator.identifyTarget();
	2dcoord targetPosition = navigator.getTargetPosition();
}

void Behaviours::moveToTarget(){
	int bufferzone = 5; //cm in front of target
	motors.rotateAngle(targetPosition.theta);
	motors.moveToPosition(targetPosition.r - bufferzone);
}

void Behaviours::grabTarget(){
	//arm.moveToTarget()
	//arm.graspTarget()
}