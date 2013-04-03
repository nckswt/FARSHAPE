#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "FSObject.h"
#include <vector>
#include "includes.h"

//Add function that rebuilds structure object from data should master robot go down
class Structure: public FSObject
{
private:
	//Use vector of vectors?
	std::vector<std::vector<float> > Build;
	std::vector<float> *Piece;


public:
	//structure's position is defined by the center of the first beam
	Structure(Position intial_position);
	
	int nextPiece(int* tt, float* tx, float* ty, float* tz);//Checks the next available spot (Status must be 0 and ID must be 0)
	void getPiece(int* tt, float* tx, float* ty, float* tz);
	void loadPiece(int buildorder, int stat, int id);//To load the status and ID of the bar
	void updatePiece(float x, float y, float z);//May not be needed
	//Add function for a robot to check the current state of the structure
	~Structure();
};

#endif