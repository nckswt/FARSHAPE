#include "FSObject.h"

int FSObject::next_ID = 0;

FSObject::FSObject(Position initial_position, ObjectType object_type, std::string object_name = ""){
	position.x = initial_position.x;
	position.y = initial_position.y;
	position.z = initial_position.z;
	position.r = initial_position.r;
	name = object_name;
	type = object_type;
	ID = ++next_ID;
}

void FSObject::get_Position(Position* updated_position){
	updated_position->x = position.x;
	updated_position->y = position.y;
	updated_position->z = position.z;
	updated_position->r = position.r;
}

int FSObject::get_ID(){
	return ID;
}

void FSObject::set_Position(Position new_position){
	position.x = new_position.x;
	position.y = new_position.y;
	position.z = new_position.z;
	position.r = new_position.r;
}

void FSObject::reset_ID(int new_ID){
	next_ID = new_ID;
}

int FSObject::get_NextID(){
	return next_ID;
}