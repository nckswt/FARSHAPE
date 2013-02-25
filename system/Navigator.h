#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <string>

struct SphCoord{
	double r;
	double theta;
	double phi;
};

struct Cart3DCoord{
	double x;
	double y;
	double z;
};

struct ZoneEdges{
	double north;
	double south;
	double east;
	double west;
};

class Navigator{
public:
	Navigator();
	~Navigator();
	void identifyTarget(std::string imageFile);
	Cart3DCoord* identifyObjects();
	Cart3DCoord* mapObjectPositions();
	Cart3DCoord mapStructurePosition();
	SphCoord cart2spherical(Cart3DCoord position);
	Cart3DCoord getTargetPosition();

private:
	ZoneEdges mapZoneEdges();

	ZoneEdges zoneEdges;
	Cart3DCoord* otherRobotPositions;
	Cart3DCoord* memberPositions;
	Cart3DCoord structurePosition;
	Cart3DCoord targetPosition;
	int targetType;
};

#endif