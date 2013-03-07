#ifndef _BEHAVIOURS_H_
#define _BEHAVIOURS_H_
//#include "Actuators.h"
#include "Behaviours.h"
//#include "Sensors.h"
#include "Navigator.h"
#include "../mobility/Motors.h"
#include "../location/Location.h"


//Behaviours are atomic commands that each robot can perform. A behaviour list is a sequential form of Behaviours
class Behaviours{
public:
	Behaviours();
	~Behaviours();

	//Atomic Behaviours
	void setupActuatorsAndSensors();
	void goDistance(double targetDistance, int speed);
	void rotateAngle(double targetAngle, int speed);
	Cart3DCoord findTarget();
	void moveToTarget(SphCoord targetPosition);
	void grabTarget(Cart3DCoord targetPosition);

	//Behaviour Lists
	void trackTarget();

	//sensor and actuator classes
	Motors motors;
	Navigator navigator;
	//Encoders encoders;
	//Arm arm;
};

#endif