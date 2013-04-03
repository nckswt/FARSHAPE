#ifndef COMMANDER_H
#define COMMANDER_H

#include "ros/ros.h" //Uncomment when uploaded on raspberry pi's
#include "Structure.h"
#include <stdlib.h>
#include "includes.h"

class Commander : public FSObject
{
private:
	FSObject target; //commander's current target object
	int priority; //this specific robot's position in the robot hierarchy
	int number_of_robots; //max of 3 robots. less if one deactivates.
	RobotMode mode; //current mode (none, explorer, builder or inspector)
	Structure* structure;
	std::vector<FSObject*> piece_locations;
	
	//ROS objects
	ros::NodeHandle* n;
    ros::Rate* loop_rate;

	//ROS Comm Objects
	std::vector<ros::Publisher*> transmitters;
	ros::Subscriber sub;
	sensor_msgs::JointState localRx;


public:
	Commander(Position initial_position, std::string robot_name, int argc, char **argv) : FSObject(initial_position, ROBOT, robot_name);
  void setupComms(); //initialize communications
  void communicate(std::string,float,float,float); //send & receive messages
  void readCommunications(); //interpret incoming data
  int checkVitals(); //failure codes represented by integer, there may not be anything to check by the demo...skip for now
  void explore();//Go into explorer mode, define search area
  void build();//Go into build mode and place a part
  void inspect();//Go into inspector mode and verify construction

};

#endif
