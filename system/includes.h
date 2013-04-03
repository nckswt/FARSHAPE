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
	enum ObjectType { UNKNOWN, ROBOT, BEAM, COLUMN, STRUCTURE };

	enum RobotMode {NO_MODE, EXPLORER_MODE, BUILDER_MODE, INSPECTOR_MODE}

	const float kSearchRadius = 2.0; //(2 meters)



#endif //INCLUDES_H