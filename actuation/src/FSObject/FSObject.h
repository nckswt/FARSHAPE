#ifndef FSOBJECT_H
#define FSOBJECT_H

#include <string>




class FSObject
{
protected:
	int ID;
	static int nextID;
	std::string Name;
	enum ObjType {Robot, Beam, Column, Structure, Unknown};
	ObjType type;
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
	FSObject(float,float,float);
	Coordinates get_Position();
	void reset_Position(float,float,float);
	int	get_ID();
	~FSObject();
};


#endif