#ifndef COMMANDER_H
#define COMMANDER_H

#include "ros/ros.h" //Uncomment when uploaded on raspberry pi's
#include "Structure.h"
#include <stdlib.h>
#include <math.h>

#define ROS_RATE 10

const float kSearchRadius = 2.0; //(2 meters)
enum modes_t {NO_MODE, EXPLORER_MODE, BUILDER_MODE, INSPECTOR_MODE}

class Commander : public FSObject
{
private:
  Commander(float xi, float yi, float zi, std::string robot_name, int argc, char **argv) : FSObject(xi, yi, zi, rName);
  float target_x; //x position of center of target object
  float target_y; //y position of center of target object
  float target_z; //z position of center of target object
  int priority; //order in robot hierarchy
  int number_of_robots; //max of 3 robots. Less if one deactivates.
  enum modes_t mode; //current mode (none, explorer, builder or inspector) 
  Structure* structure; 
  std::vector<FSObject*> piece_locations;
  
  //ROS objects
  ros::NodeHandle* n;
  ros::Rate* loop_rate;
  
  //ROS Comm Objects
  std::vector<ros::Publisher*> transmitters;
  ros::Subscriber sub;
  sensor_msgs::JointState Tx;
  
public:
  void setupComms(); //initialize communications
  void communicate(std::string,float,float,float); //send & receive messages
  void readCommunications(); //interpret incoming data
  int checkVitals(); //failure codes represented by integer, there may not be anything to check by the demo...skip for now
  void explore();//Go into explorer mode, define search area
  void build();//Go into build mode and place a part
  void inspect();//Go into inspector mode and verify construction
};

#endif //COMMANDER_H
