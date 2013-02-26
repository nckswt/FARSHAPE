#include "Navigator.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265

Navigator::Navigator(){

}

Navigator::~Navigator(){

}

void Navigator::identifyTarget(std::string imageFile){
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
}

Cart3DCoord* Navigator::identifyObjects(){

}

Cart3DCoord* Navigator::mapObjectPositions(){

}

Cart3DCoord Navigator::mapStructurePosition(){

}

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