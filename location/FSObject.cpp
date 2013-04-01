#include "FSObject.h"

int FSObject::nextID = 0;

void FSObject::get_Position(float* tx, float* ty, float* tz)
{
	*tx = position.x;
	*ty = position.y;
	*tz = position.z;
}

int FSObject::get_ID()
{
	return ID;
}

void FSObject::set_Position(float xi, float yi, float zi)
{
	position.x = xi;
	position.y = yi;
	position.z = zi;
}

FSObject::FSObject(float xi, float yi, float zi, int what, std::string name = "")
{
	Name = name;
	type = what;
	position.x = xi;
	position.y = yi;
	position.z = zi;
	ID = ++nextID;

}

void FSObject::reset_ID(int newID)
{
	nextID = newID;
}

int FSObject::get_NextID()
{
	return nextID;
}