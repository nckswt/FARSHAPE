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
	//const int Beam = 1;
	//const int Column = 2;

public:
	//Make it such that it loads this from file (if there is time)
	Structure(float xi, float yi, float zi) : FSObject(xi, yi, zi, 3, "Mike Town") //Maybe refer to center of cube? in which case xi and yi need to offset
	{
		Piece = new std::vector<float>;
		Piece->push_back(Beam);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved, 1 is for busy, 2 is for placed
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Column);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Beam);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi - 0.169);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Column);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Beam);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Column);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Beam);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi - 0.169);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(Column);	//Is it a beam or column?
		Piece->push_back(0);	//Status, 0 is for not reserved
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);
	}
	
	int next_Piece(int* tt, float* tx, float* ty, float* tz);//Checks the next available spot (Status must be 0 and ID must be 0)
	void get_Piece(int* tt, float* tx, float* ty, float* tz);
	void load_Piece(int buildorder, int stat, int id);//To load the status and ID of the bar
	void update_Piece(float x, float y, float z);//May not be needed
	//Add function for a robot to check the current state of the structure
	~Structure();
};

#endif