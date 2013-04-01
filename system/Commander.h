#ifndef COMMANDER_H
#define COMMANDER_H

#include "ros/ros.h" //Uncomment when uploaded on raspberry pi's
#include "Structure.h"
#include <stdlib.h>

#define ROS_RATE 10

const float searchRadius = 2.0; //(2 meters)
const float c_PI = 3.14159;//May not need

class Commander : public FSObject
{
private:
  float target_x;
  float target_y;
  float target_z;
  bool master;
  int numRobots;
  Structure* strctr;
  std::vector<FSObject*> locPieces;
  
  //ROS objects
  ros::NodeHandle* n;
  ros::Rate* loop_rate;
  
  //ROS Comm Objects
  std::vector<ros::Publisher*> transmitters;
  ros::Subscriber sub;
  sensor_msgs::JointState Tx;
  
  
  
public:
  Commander(float xi, float yi, float zi, std::string rName, int argc, char **argv) : FSObject(xi, yi, zi, rName) //Maybe pull this information from a text file on each robot.
  {
    //On startup (creation of robot)
    //Check vitals (probably nothing to do at this point)
    //Since its Ash, roscore and rosinit need to be instantiated
    if (Name == "Ash") // && other robots haven't defined themselves as master (if it is reset) <- implement if theres time
    {
      master = true;
      
      // Is the following necessary? System calls are fucking bad. 
      system("run_core.sh");//used to run roscore, setup, ROS_MASTER_URI etc,
    }
    else
    {
      //If slave then run another script, need to deal with when the master changes
    }
    //TODO: Create comms object and ping other robots
    //TODO: Any other checks performed here
    //TODO: Create structure object at (0,0,0)
    
    //TODO: Maybe add code to check other robots for current mission should the 
    //      robot reset due to power issues
    
    //ROS SETUP****************************************
    ros::init(argc, argv, Name);
    n = new ros::NodeHandle;
    loop_rate = new ros::Rate(ROS_RATE);
    numRobots = 3;//Default
  }
  
  void setup_Comms();
  void use_Comms(std::string,float,float,float);
  void check_Comms();
  int check_Vitals(); //failure codes represented by integer, there may not be anything to check by the demo...skip for now
  void search_Pieces();//Go into search mode, define search area
  void build_Structure();//once pieces are found, go
  void check_Structure();//Verify after piece has been placed (may not have time to implement this properly)
  
};

#endif
