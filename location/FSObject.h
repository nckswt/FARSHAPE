#ifndef FSOBJECT_H
#define FSOBJECT_H

#include <string>
#include <iostream>

class FSObject
{
protected:
	int ID;
	static int nextID;
	std::string Name;
	
	int type;
	/*
		0 - Robot
		1 - Beam
		2 - Column
		3 - Structure
	   -1 - Unknown
	*/

	//Structure position represents the xyz of first member?
	struct Coordinates 
	{
		float x;
		float y;
		float z;
	};
	Coordinates position;
	float length, width, height;

public: 
	FSObject(float,float,float,int,std::string name = "");
	void get_Position(float* tx, float* ty, float* tz);
	void set_Position(float,float,float);
	int get_NextID();
	void reset_ID(int);
	int	get_ID();
	//~FSObject();
};


#endif