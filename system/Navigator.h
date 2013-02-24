#import <string>

struct SphericalCoordinate{
	double r;
	double theta;
	double phi;
};

struct Cart2D{
	double x;
	double y;
};

struct Cart3D{
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
	void identifyTarget(string);
	Cart3D* identifyObjects();
	Cart3D* mapObjectPositions();
	Cart3D mapStructurePosition();
	SphericalCoordinate rect2spherical(Cart3D);

private:
	ZoneEdges mapZoneEdges();

	ZoneEdges zoneEdges;
	Cart3D* otherRobotPositions;
	Cart3D* memberPositions;
	Cart3D structurePosition;
	Cart3D targetPosition;
	int targetType;
};
