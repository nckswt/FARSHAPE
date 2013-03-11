#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "FSObject.h"
#include <vector>

class Structure: public FSObject
{
private:
	//Use vector of vectors?
	std::vector<std::vector<float> > Build;
	std::vector<float> *Piece;

public:
	Structure(float xi, float yi, float zi) : FSObject(xi, yi, zi)
	{
		Piece = new std::vector<float>;
		Piece->push_back(Beam);
		Piece->push_back(xi);
		Piece->push_back(yi);
		Piece->push_back(zi);
		Build.push_back(*Piece);
	}
	//Structure();

	~Structure();
};

#endif