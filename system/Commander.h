#ifndef COMMANDER_H
#define COMMANDER_H

#include <ros/ros.h> //Uncomment when uploaded on raspberry pi's
#include <sensor_msgs/JointState.h>
#include "Structure.h"
#include <stdlib.h>
#include "includes.h"
#include "FSObject.h"
#include <math.h>
#include "Helm.h"
#include "../sensing/camera/camera.h"


class Commander : public FSObject{
private:
	FSObject target; //commander's current target object
	Position position_target;

	int priority; //this specific robot's position in the robot hierarchy
	bool is_master;
	bool verify;
	int number_of_robots; //max of 3 robots. less if one deactivates.
	Structure* structure;
	Helm* helm;
	Camera* camera;
	std::vector<FSObject*> piece_locations;
	
	//ROS objects
	//ros::NodeHandle* n;
    //ros::Rate* loop_rate;

	//ROS Comm Objects
	//std::vector<ros::Publisher*> transmitters;
	//ros::Subscriber sub;
	sensor_msgs::JointState localRx;

public:
  RobotMode mode; //current mode (none, explorer, builder or inspector)
  bool structure_is_complete;

  Commander(Position initial_position, std::string robot_name,int argc, char **argv);
  
  void setupComms(); //initialize communications
  void communicate(std::string,float,float,float,float); //send & receive messages
  void readCommunications(); //interpret incoming data
  int checkVitals(); //failure codes represented by integer, there may not be anything to check by the demo...skip for now
  void explore();//Go into explorer mode, define search area
  void build();//Go into build mode and place a part
  void inspect();//Go into inspector mode and verify construction

};

#endif
