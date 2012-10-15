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
	enum ObjectType {ROBOT_TYPE = 0, BEAM_TYPE = 1, COLUMN_TYPE = 2, STRUCTURE_TYPE = 3, UNKNOWN_TYPE = -1};
	enum MemberStatus {NOT_RESERVED = 0, BUSY = 1, PLACED = 2};
	enum RobotMode {NO_MODE, EXPLORER_MODE, BUILDER_MODE, INSPECTOR_MODE};

	const float kSearchRadius = 2.0; //(2 meters)
	const float kMaxIRRange = 30.0; //30cms 



#endif //INCLUDES_H