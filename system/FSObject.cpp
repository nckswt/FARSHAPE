#include "FSObject.h"

int FSObject::next_ID = 0;

FSObject::FSObject(){
	
}

FSObject::FSObject(Position initial_position, ObjectType object_type, std::string object_name = ""){
	position.x = initial_position.x;
	position.y = initial_position.y;
	position.z = initial_position.z;
	position.r = initial_position.r;
	name = object_name;
	type = object_type;
	ID = ++next_ID;
}

void FSObject::getPosition(Position* updated_position){
	updated_position->x = position.x;
	updated_position->y = position.y;
	updated_position->z = position.z;
	updated_position->r = position.r;
}

int FSObject::getID(){
	return ID;
}

void FSObject::setPosition(Position new_position){
	position.x = new_position.x;
	position.y = new_position.y;
	position.z = new_position.z;
	position.r = new_position.r;
}

void FSObject::resetID(int new_ID){
	next_ID = new_ID;
}

int FSObject::getNextID(){
	return next_ID;
}