#include "FSObject.h"

int FSObject::next_ID = 0;

FSObject::FSObject(float xi, float yi, float zi, int what, std::string name = "")
{
	Name = name;
	type = what;
	position.x = xi;
	position.y = yi;
	position.z = zi;
	ID = ++next_ID;

}

void FSObject::getPosition(float* tx, float* ty, float* tz)
{
	*tx = position.x;
	*ty = position.y;
	*tz = position.z;
}

int FSObject::getID()
{
	return ID;
}

void FSObject::setPosition(float xi, float yi, float zi)
{
	position.x = xi;
	position.y = yi;
	position.z = zi;
}

void FSObject::resetID(int newID)
{
	next_ID = newID;
}

int FSObject::getNextID()
{
	return next_ID;
}