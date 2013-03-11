/****************************NOTES AND IDEAS

To implement simple collision detection, reduce the range of operation of the arm,
such that the robot positions itself directly infront of the desired location

*/


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

using namespace std;

  void servoCallBack(const sensor_msgs::JointState::ConstPtr& msg)
  {
    cout << endl;
    for (int j = 0; j < 8; j++)
    {
      cout << "Servo: " << j + 1 << "  " << msg->position[j] << endl;
    }
    cout << endl;
  }


/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  const double STEP_SIZE_ARM = 0.01;
  //const double STEP_SIZE_WRIST = 0.014;
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "test");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */



  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */




  ros::Publisher wr_rot_pub = n.advertise<std_msgs::Float64>("/wrist_rotate/command/", 1000);

  ros::Publisher bs_rot_pub = n.advertise<std_msgs::Float64>("/base_rotate/command/", 1000);

  ros::Publisher bs_pit1_pub = n.advertise<std_msgs::Float64>("/base_pitch1/command/", 1000);

  ros::Publisher bs_pit2_pub = n.advertise<std_msgs::Float64>("/base_pitch2/command/", 1000);

  ros::Publisher ar_pit1_pub = n.advertise<std_msgs::Float64>("/arm_pitch1/command/", 1000);

  ros::Publisher ar_pit2_pub = n.advertise<std_msgs::Float64>("/arm_pitch2/command/", 1000);

  ros::Publisher wr_pit_pub = n.advertise<std_msgs::Float64>("/wrist_pitch/command/", 1000);

  ros::Publisher gp_lnr_pub = n.advertise<std_msgs::Float64>("/gripper_width/command/", 1000);

  ros::Subscriber servo_feedback = n.subscribe<sensor_msgs::JointState>("/joint_states", 1000, servoCallBack);

  ros::Rate loop_rate(2);
  ros::Rate high_rate(20);


   /**************************LOAD ROBOT*************************************
   KDL::Tree my_tree;
   ros::NodeHandle node;
   string robot_desc_string;
   node.param("robot_description", robot_desc_string, string());
   if (!kdl_parser::treeFromString(robot_desc_string, my_tree))
   {
      ROS_ERROR("Failed to construct kdl tree");
      return false;
   }


    //Creation of the chain:
   //It is possible to request the chain in a tree between a certain root and a tip:
  bool exit_value;
  KDL::Chain chain1;
  exit_value = my_tree.getChain("base_link","arm2",chain1);
  //Segment 1 and segment 3 are included but segment 1 is renamed.

   //*****************************END ******************************/




  KDL::Chain chain1;

  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame(KDL::Rotation::RPY(0,0,0),KDL::Vector(0.0,0.0,0.03))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0.0,0.0),KDL::Vector(0,0.0,0.17))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.17,0.0,0.0))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.12,0.0,0))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.0,0.0,0))));
  //chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.0,0.0,0.1))));
  //chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0.0,0.0,0.120))));
 

  
  //INVERSE KINEMATICS*****************************************************************


  //Creation of the solvers:
  KDL::ChainFkSolverPos_recursive fksolver1(chain1);//Forward position solver
  KDL::ChainIkSolverVel_pinv iksolver1v(chain1);//Inverse velocity solver
  KDL::ChainIkSolverPos_NR iksolver1(chain1,fksolver1,iksolver1v,100,1e-4);//Maximum 100 iterations, stop at accuracy 1e-6
  


  //Creation of jntarrays:
  KDL::JntArray q(chain1.getNrOfJoints());
  KDL::JntArray q_init(chain1.getNrOfJoints());
  time_t now = time(0);
  cout << now << endl;
  cout << "Number of Joints:    ";
  cout << chain1.getNrOfJoints() << endl;
  cout << endl;

  //Set destination frame
  KDL::Frame F_dest = KDL::Frame(KDL::Rotation::RPY(0, (90*M_PI/180), 0),KDL::Vector(0.30, 0, 0.05));
  

  int ret = iksolver1.CartToJnt(q_init,F_dest,q);
  cout << "Status of iksolver1.CartToJnt:   ";
  cout << ret << endl;

  for (int i = 0; i < chain1.getNrOfJoints(); i++)
  {
    q_init.data[i] = 0.01;
    //cout << "q_init.data[" << i << "] =    " << q_init.data[i] << endl;
  }

  cout << endl;


  for (int i = 0; i < chain1.getNrOfJoints(); i++)
  {
    cout << "q.data[" << i << "] =         " << q.data[i] << "      " << (180*q.data[i]/M_PI) << endl;
  }

  cout << endl;
  std_msgs::Float64 msg[8];
  int counter = 0;
  double temp = 0;

  while (counter < 115)
  {
      for (int j = 0; j < q.rows(); j++) 
      {
            msg[j].data = q.data[j];
            while (msg[j].data > (2*M_PI) || msg[j].data < (-2*M_PI))
            {
              if (msg[j].data > 0)
              {
                msg[j].data = msg[j].data - 2*M_PI;
              }
              else
              {
                msg[j].data =msg[j].data + 2*M_PI;
              }
        }
            //cout << msg[j].data << endl;
      }



     
     msg[3].data = -1*(msg[3].data);//Wrist pitch
     msg[5].data = -1*(msg[2].data);//Arm pitch follow
     msg[4].data = -1*(msg[1].data);//Base pitch follow
     msg[6].data = 0;
     
     //Just for fun
    if (counter > 103)
    {
        msg[1].data = 0;
        msg[4].data = 0;
    }

     //Instead of delays, we can check to see if the servo has reached the target position!
    if (counter < 3)
    {


      msg[7].data = 0;//Gripper open
      msg[3].data = 0;//Wrist pitch
      msg[2].data = -M_PI/4;//Arm pitch control
      msg[5].data = M_PI/4;//Arm pitch follow

    }
    else
    {

      bs_rot_pub.publish(msg[0]);
 
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);
    }

    if (counter > 3 && counter < 104)
    {
      //Possibly add feedback to make this more accurater
      msg[2].data = ((counter*STEP_SIZE_ARM)*(msg[2].data) + ((-1*M_PI/2) + (counter*STEP_SIZE_ARM*M_PI/2)));//Arm Pitch Control
      msg[5].data = ((counter*STEP_SIZE_ARM)*(msg[5].data) + ((M_PI/2) - (counter*STEP_SIZE_ARM*M_PI/2)));//Arm pitch follow
      //Edit this line such that it acts as those above, with still handling the 90degree conidition
      msg[3].data = ((-1*M_PI/2) + msg[2].data + msg[1].data);//Wrist pitch

      cout << endl;
      cout << "Lead Pitch: " << msg[2].data;
      cout << endl;
      cout << "Follow Pitch: " << msg[5].data;
      cout << endl;
      cout << "Wrist: " << msg[3].data;
      cout << endl;

      if (counter > 100)
      {
        msg[7].data = M_PI/2;//Gripper close
        sleep(2);
      }
    }



    wr_pit_pub.publish(msg[3]);
    //sleep(1);
    gp_lnr_pub.publish(msg[7]);
    ar_pit2_pub.publish(msg[2]);
    ar_pit1_pub.publish(msg[5]);
    wr_rot_pub.publish(msg[6]);

    
    ros::spinOnce();
    if (counter < 3)
    {
      loop_rate.sleep();
      sleep(1);
    }
    else
    {
      high_rate.sleep();
    }
    counter++;



  }
  
  return 0;
}
