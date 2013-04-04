#ifndef FSOBJECT_H
#define FSOBJECT_H

#include <string>
#include <iostream>
#include "includes.h"


class FSObject{
protected:
	int ID; //object ID
	static int next_ID; //unassigned next ID. Updated whenever an ID is assigned.
	std::string name; //identifying name
	ObjectType type; //unknown, robot, beam, column, structure
	Position position; //x,y,z,r
	Dimensions dimensions; //l,w,h

public: 
	FSObject(); //<>CHECK: WHY THE HELL DOES THIS DEFAULT CONSTRUCTOR WORK?
	FSObject(Position initial_position, ObjectType type, std::string object_name);
	void getPosition(Position* updated_position);
	void setPosition(Position new_position);
	int getNextID();
	void resetID(int new_ID);
	int	getID();
	ObjectType getType();
	std::string getName();
	//~FSObject();
};


#endif