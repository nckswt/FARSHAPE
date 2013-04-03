#ifndef INCLUDES_H
#define INCLUDES_H

	struct Position {
		float x;
		float y;
		float z;
		float r; //rotation angle
	};

	struct Dimensions{
		float length;
		float width;
		float height;
	};

	// Defines the types used
	enum ObjectType {ROBOT = 0, BEAM = 1, COLUMN = 2, STRUCTURE = 3, UNKNOWN = -1};

	enum RobotMode {NO_MODE, EXPLORER_MODE, BUILDER_MODE, INSPECTOR_MODE}

	const float kSearchRadius = 2.0; //(2 meters)



#endif //INCLUDES_H