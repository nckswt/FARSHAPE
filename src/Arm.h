#ifndef ACTUATION_H
#define ACTUATION_H

#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include <kdl_parser/kdl_parser.hpp>
#include <sstream>
#include <string>
#include <iostream>
#include <kdl/chain.hpp>
#include <kdl/chainfksolver.hpp>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include <kdl/frames_io.hpp>
#include <kdl/frames.hpp>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <sensor_msgs/JointState.h>
#include <math.h>

using namespace std;

class Arm
{
private:
	//Variables
	std_msgs::Float64 msg[8];
	string action;

  	ros::Publisher wr_rot_pub;
  	ros::Publisher bs_rot_pub;
  	ros::Publisher bs_pit1_pub;
  	ros::Publisher bs_pit2_pub;
  	ros::Publisher ar_pit1_pub;
  	ros::Publisher ar_pit2_pub;
  	ros::Publisher wr_pit_pub;
  	ros::Publisher gp_lnr_pub;

  	ros::Subscriber servo_feedback;

  	ros::NodeHandle* n;

    ros::Rate* loop_rate;
    ros::Rate* high_rate;
  	
  	//KDL
  	KDL::Chain chain1;
  	KDL::ChainFkSolverPos_recursive* fksolver1;
  	KDL::ChainIkSolverVel_pinv* iksolver1v;
  	KDL::ChainIkSolverPos_NR* iksolver1;

  	KDL::JntArray* q;
    KDL::JntArray* q_init;

    KDL::Frame F_dest;


public:
  //To be used by callback
  //double servo_pos[8];
	//Functions
	Arm(int, char**);

  int actuate_Arm(double, double, double, string);
	void relax_Arm();
	void purgatory_Arm();
	int interact_Arm(double, double, double, string);
	//void servoCallBack(const sensor_msgs::JointState::ConstPtr& msg);
	
	~Arm();

};





#endif