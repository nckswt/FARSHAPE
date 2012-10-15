#include "Structure.h"

	Structure::Structure(Position intial_position) : FSObject(intial_position, STRUCTURE_TYPE, "Mike Town"){
		float xi = intial_position.x;
		float yi = intial_position.y;
		float zi = intial_position.z;


		Piece = new std::vector<float>;
		Piece->push_back(BEAM_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status, 1 is for busy, 2 is for placed
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(COLUMN_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(BEAM_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi - 0.169);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(COLUMN_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi + 0.169);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(BEAM_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(COLUMN_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi - 0.338);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(BEAM_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi - 0.169);
		Piece->push_back(zi);
		Build.push_back(*Piece);

		Piece = new std::vector<float>;
		Piece->push_back(COLUMN_TYPE);	//Is it a beam or column?
		Piece->push_back(NOT_RESERVED);	//Status
		Piece->push_back(0);	//ID of bar in this place, ID 0 is for "not reserved"
		Piece->push_back(xi - 0.169);	//Target coordinates
		Piece->push_back(yi);
		Piece->push_back(zi + 0.176);
		Build.push_back(*Piece);
	}


int Structure::nextPiece(int* tt, float* tx, float* ty, float* tz){
	int count = 0;
	for(std::vector<std::vector<float> >::iterator it = Build.begin(); it != Build.end(); ++it) 
	{
		if ((*it)[2] == 0 && (*it)[1] == 0)//Next bar to go in, should there be anything about status? probably not for this.
		{
			*tt = int((*it)[0]);//Tells robot if it needs to get a beam or column
			*tx = (*it)[3];//coordinates x,y,z
			*ty = (*it)[4];
			*tz = (*it)[5];
			(*it)[1] = 1;//Load status of bar to busy
			return count; //Returns position in build order
		}
		count++;
	}
	return -1;//if all bars are being placed or already placed
}

void Structure::loadPiece(int buildorder, int stat, int id)
{
	(Build[buildorder])[1] = stat; //Placed or busy
	(Build[buildorder])[2] = id; //ID of bar to be placed
}

void Structure::getPiece(int pos, int* tt, float* tx, float* ty, float* tz)
{
	*tt = int(Build[pos][0]);
	*tx = Build[pos][3];
	*ty = Build[pos][4];
	*tz = Build[pos][5];
}