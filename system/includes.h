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

	enum ObjectType { UNKNOWN_TYPE, ROBOT_TYPE, BEAM_TYPE, COLUMN_TYPE, STRUCTURE_TYPE };
	enum RobotMode {NO_MODE, EXPLORER_MODE, BUILDER_MODE, INSPECTOR_MODE};

	const float kSearchRadius = 2.0; //(2 meters)



#endif //INCLUDES_H