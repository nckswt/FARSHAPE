#include "FSObject.h"

int FSObject::nextID = 0;

Coordinates FSObject::get_Position()
{
	return position;
}

int FSObject::get_ID()
{
	return ID;
}

void FSObject::reset_Position(float xi, float yi, float zi)
{
	position.x = xi;
	position.y = yi;
	position.z = zi;
}

FSObject::FSObject(float xi, float yi, float zi)
{
	position.x = xi;
	position.y = yi;
	position.z = zi;
	ID = ++nextID;

}