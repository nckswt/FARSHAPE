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
#include <math.h>

using namespace std;

  double servo_pos[8];

  void servoCallBack(const sensor_msgs::JointState::ConstPtr& msg)
  {
    for (int j = 0; j < 8; j++)
    {
      //cout << "Servo: " << j + 1 << "  " << msg->position[j] << endl;
    }
    servo_pos[0] = msg->position[4];  //Base rotate
    servo_pos[1] = msg->position[6];  //Base pitch lead
    servo_pos[2] = msg->position[3];  //Arm pitch lead
    servo_pos[3] = msg->position[0];  //Wrist pitch
    servo_pos[4] = msg->position[5];  //Base pitch follow
    servo_pos[5] = msg->position[2];  //Arm pitch follow
    servo_pos[6] = msg->position[7];  //Wrist rotate
    servo_pos[7] = msg->position[1];  //Gripper
  }


  std_msgs::Float64 msg[8];


  ros::Publisher wr_rot_pub;
  ros::Publisher bs_rot_pub;
  ros::Publisher bs_pit1_pub;
  ros::Publisher bs_pit2_pub;
  ros::Publisher ar_pit1_pub;
  ros::Publisher ar_pit2_pub;
  ros::Publisher wr_pit_pub;
  ros::Publisher gp_lnr_pub;

  //KDL CHAIN INIT
  KDL::Chain chain1;

  
  //chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.0,0.0,0.1))));
  //chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Vector(0.0,0.0,0.120))));

const double STEP_SIZE_ARM = 0.01;

void move_Arm(double,double,double,string,ros::NodeHandle,ros::Rate, ros::Rate);
void purgatory_Arm(ros::NodeHandle,ros::Rate,ros::Rate);
void pickup_Object(double,double,double, string, ros::NodeHandle,ros::Rate, ros::Rate);
void relax_Arm(ros::NodeHandle,ros::Rate,ros::Rate);
/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  
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

  //This would be in the constructor!!!!!
  ros::init(argc, argv, "test");

  //ROS INIT
  ros::NodeHandle n;

  ros::Rate loop_rate(2);
  ros::Rate high_rate(1000);//Works at 1000

  wr_rot_pub = n.advertise<std_msgs::Float64>("/wrist_rotate/command/", 1000);
  bs_rot_pub = n.advertise<std_msgs::Float64>("/base_rotate/command/", 1000);
  bs_pit1_pub = n.advertise<std_msgs::Float64>("/base_pitch1/command/", 1000);
  bs_pit2_pub = n.advertise<std_msgs::Float64>("/base_pitch2/command/", 1000);
  ar_pit1_pub = n.advertise<std_msgs::Float64>("/arm_pitch1/command/", 1000);
  ar_pit2_pub = n.advertise<std_msgs::Float64>("/arm_pitch2/command/", 1000);
  wr_pit_pub = n.advertise<std_msgs::Float64>("/wrist_pitch/command/", 1000);
  gp_lnr_pub = n.advertise<std_msgs::Float64>("/gripper_width/command/", 1000);
  ros::Subscriber servo_feedback = n.subscribe<sensor_msgs::JointState>("/joint_states", 1000, servoCallBack);

  
  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  

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

  

   //Build chain from robot parameters
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame(KDL::Rotation::RPY(0,0,0),KDL::Vector(0.0,0.0,0.04))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0.0,0.0),KDL::Vector(0,0.0,0.15))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::None),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.02,0.0,0))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.16,0.0,0.0))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.13,0.0,0))));
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.0,0.0,0))));
  
/***************REMOVE COMMENT*************************
  move_Arm(0.28,0,-0.03,"Grab",n,loop_rate,high_rate);


  //***************************RELAX WITH BAR************************************
  msg[0].data = 0;//Base Rotate
  msg[1].data = -1*M_PI/2;//Base Pitch Lead
  msg[2].data = M_PI/3;//Arm Pitch Lead
  msg[3].data = 0; //Wrist Pitch

  msg[3].data = -1*(msg[3].data);//Wrist pitch
  msg[5].data = -1*(msg[2].data);//Arm pitch follow
  msg[4].data = -1*(msg[1].data);//Base pitch follow

  wr_pit_pub.publish(msg[3]);
  sleep(1);
  gp_lnr_pub.publish(msg[7]);
  ar_pit2_pub.publish(msg[2]);
  ar_pit1_pub.publish(msg[5]);
  wr_rot_pub.publish(msg[6]);
  bs_pit1_pub.publish(msg[1]);
  bs_pit2_pub.publish(msg[4]);

  ros::spinOnce();
  sleep(2);

  msg[2].data = M_PI/2;//Arm Pitch Lead
  msg[5].data = -1*(msg[2].data);//Arm pitch follow
  ar_pit2_pub.publish(msg[2]);
  ar_pit1_pub.publish(msg[5]);

  ros::spinOnce();
  sleep(3);

  
    /*
  arbotix_msgs::Relax serv;

  ros::service::call("/wrist_rotate/relax",serv);
  ros::service::call("/base_rotate/relax",serv);
  ros::service::call("/base_pitch1/relax",serv);
  ros::service::call("/base_pitch2/relax",serv);
  ros::service::call("/arm_pitch1/relax",serv);
  ros::service::call("/arm_pitch2/relax",serv);
  ros::service::call("/wrist_pitch/relax",serv);
  */

//**********************************************************************************


//*********************PLACE BAR***************************************************


//move_Arm(0.20,0,0.2,"Place",n,loop_rate,high_rate);

//sleep(15);


  //**********************************************************************************
  purgatory_Arm(n,loop_rate, high_rate);
  pickup_Object(0.2, 0.0, -0.035, "Grab",n,loop_rate,high_rate);
  relax_Arm(n,loop_rate,high_rate);
  purgatory_Arm(n,loop_rate, high_rate);
  pickup_Object(0.31, 0.0, 0.28, "Place",n,loop_rate,high_rate);
  relax_Arm(n,loop_rate,high_rate);
  //sleep(10);
  return 0;
}

/*
  void move_Arm(double x, double y, double z, string action,ros::NodeHandle n,ros::Rate loop_rate,ros::Rate high_rate)
  {

    //INVERSE KINEMATICS*****************************************************************

    //Creation of the solvers:
    KDL::ChainFkSolverPos_recursive fksolver1(chain1);//Forward position solver
    KDL::ChainIkSolverVel_pinv iksolver1v(chain1);//Inverse velocity solver
    KDL::ChainIkSolverPos_NR iksolver1(chain1,fksolver1,iksolver1v,100,1e-6);//Maximum 100 iterations, stop at accuracy 1e-6
  
    //Creation of jntarrays:
    KDL::JntArray q(chain1.getNrOfJoints());
    KDL::JntArray q_init(chain1.getNrOfJoints());
    //***************************************************************************************

    KDL::Frame F_dest;


    cout << "Number of Joints:    ";
    cout << chain1.getNrOfJoints() << endl;
    cout << endl;


    //Set destination frame
    double r = sqrt((x*x) + (y*y));
    
    if (action == "Grab")
    {
      //KDL::Frame F_dest = KDL::Frame(KDL::Rotation::RPY(0, (90*M_PI/180), 0),KDL::Vector(r, 0.0, z));
      F_dest = KDL::Frame(KDL::Rotation::RPY(0, (90*M_PI/180), 0),KDL::Vector(r, 0.0, z));
      msg[6].data = 0;//Wrist rotate
      msg[7].data = M_PI/6;//Gripper open
    }
    else if (action == "Place")
    {
      //KDL::Frame F_dest = KDL::Frame(KDL::Rotation::RPY(0, 0, 0),KDL::Vector(r, 0.0, z));
      F_dest = KDL::Frame(KDL::Rotation::RPY(0, 0, 0),KDL::Vector(r, 0.0, z));
      msg[7].data = M_PI/3;//Gripper close
      msg[6].data = M_PI/2;//Wrist rotate
    }
    
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
    
    //**************************************START ACTION********************************************
    int counter = 0;
    while (counter < 115)
    {
        for (int j = 0; j < q.rows() - 1; j++) 
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

        msg[0].data = atan2(y,x);

       
       msg[3].data = -1*(msg[3].data);//Wrist pitch
       msg[5].data = -1*(msg[2].data);//Arm pitch follow
       msg[4].data = -1*(msg[1].data);//Base pitch follow
       

       //Instead of delays, we can check to see if the servo has reached the target position!

      
      if (counter > 3 && action == "Grab")
      {
        msg[1].data = 0;
        msg[4].data = 0;
      }
    

      if (counter < 3)
      {
        msg[3].data = M_PI/4;//Wrist pitch
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
        if (action == "Grab")
        {
          //Possibly add feedback to make this more accurater
          msg[2].data = ((counter*STEP_SIZE_ARM)*(msg[2].data) + ((-1*M_PI/2) + (counter*STEP_SIZE_ARM*M_PI/2)));//Arm Pitch Control
          msg[5].data = ((counter*STEP_SIZE_ARM)*(msg[5].data) + ((M_PI/2) - (counter*STEP_SIZE_ARM*M_PI/2)));//Arm pitch follow
          //Edit this line such that it acts as those above, with still handling the 90degree conidition
          msg[3].data = ((-1*M_PI/2) + msg[2].data + msg[1].data);//Wrist pitch
          wr_pit_pub.publish(msg[3]);
          ar_pit2_pub.publish(msg[2]);
          ar_pit1_pub.publish(msg[5]);

          if (counter > 100)
          {
            msg[7].data = M_PI/3;//Gripper close
          }
        }
        else if (action == "Place")
        {
          //Possibly add feedback to make this more accurater
          msg[2].data = ((counter*STEP_SIZE_ARM)*(msg[2].data) + ((-1*M_PI/2) + (counter*STEP_SIZE_ARM*M_PI/2)));//Arm Pitch Control
          msg[5].data = ((counter*STEP_SIZE_ARM)*(msg[5].data) + ((M_PI/2) - (counter*STEP_SIZE_ARM*M_PI/2)));//Arm pitch follow
          //Edit this line such that it acts as those above, with still handling the 90degree conidition
          msg[3].data =  msg[2].data + msg[1].data;//Wrist pitch

          wr_pit_pub.publish(msg[3]);
          ar_pit2_pub.publish(msg[2]);
          ar_pit1_pub.publish(msg[5]);
          if (counter > 100)
          {
            msg[7].data = 0;//Gripper open
          }
        }
      }

          if (counter > 103 && action == "Place")
          {
            msg[7].data = 0;//Gripper open
          }

      
      gp_lnr_pub.publish(msg[7]);
    //  wr_pit_pub.publish(msg[3]);
    //  ar_pit2_pub.publish(msg[2]);
    //ar_pit1_pub.publish(msg[5]);
      wr_rot_pub.publish(msg[6]);

      usleep(50000);
      ros::spinOnce();
      if (counter < 3)
      {
        loop_rate.sleep();
        //sleep(1);
      }
      else
      {
        high_rate.sleep();
      }
      counter++;

    }
  }
  */
  void pickup_Object(double x, double y, double z, string action, ros::NodeHandle n,ros::Rate loop_rate,ros::Rate high_rate)
  {
    sleep(3);
    //INVERSE KINEMATICS*****************************************************************

    //Creation of the solvers:
    KDL::ChainFkSolverPos_recursive fksolver1(chain1);//Forward position solver
    KDL::ChainIkSolverVel_pinv iksolver1v(chain1);//Inverse velocity solver
    KDL::ChainIkSolverPos_NR iksolver1(chain1,fksolver1,iksolver1v,100,1e-6);//Maximum 100 iterations, stop at accuracy 1e-6
  
    //Creation of jntarrays:
    KDL::JntArray q(chain1.getNrOfJoints());
    KDL::JntArray q_init(chain1.getNrOfJoints());
    //***************************************************************************************

    KDL::Frame F_dest;


    cout << "Number of Joints:    ";
    cout << chain1.getNrOfJoints() << endl;
    cout << endl;


    //Set destination frame
    double r = sqrt((x*x) + (y*y));
    double rotation = 0;
    if (action == "Grab")
    {
      rotation = (90*M_PI/180);
      msg[6].data = 0;//Wrist rotate
      msg[7].data = 0;//Gripper open
    }
    else if (action == "Place")
    {
      rotation = 0.0;
      msg[6].data = M_PI/2;//Wrist rotate
      msg[7].data = M_PI/2;//Gripper open
    }
    //KDL::Frame F_dest = KDL::Frame(KDL::Rotation::RPY(0, (90*M_PI/180), 0),KDL::Vector(r, 0.0, z));
    F_dest = KDL::Frame(KDL::Rotation::RPY(0, rotation, 0),KDL::Vector(r, 0.0, z));

    //Solve and check to see if it failed
    int ret = iksolver1.CartToJnt(q_init,F_dest,q);
    if (ret < 0)
    {
      while(1)
      {

      }
    }
    cout << "Status of iksolver1.CartToJnt:   ";
    cout << ret << endl;

    for (int i = 0; i < chain1.getNrOfJoints(); i++)
    {
      q_init.data[i] = 0.01;//LOAD THE LAST POSITION HERE
    }

    for (int j = 0; j < q.rows() - 1; j++) 
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
    }
    /*
    if (action == "Get")
    {
      msg[6].data = 0;//Wrist rotate
      msg[7].data = M_PI/6;//Gripper open
      
    }
    else if (action == "Place")
    {
      msg[6].data = 0;//Wrist rotate
      msg[7].data = M_PI/6;//Gripper open
    }
    */

    msg[0].data = atan2(y,x);//Orient according to angle
    bs_rot_pub.publish(msg[0]);
    gp_lnr_pub.publish(msg[7]);
    wr_rot_pub.publish(msg[6]);

    //PUT THIS INIT STUFF IN THE CONSTRUCTOR!!!! - Optimize later
    double tar_pos[8];
    double delta_pos[8];
    double start_pos[8];
    const double steps = 5000;
    double step_size = (1/steps);


    for (int i = 0; i < 8; i++)
    {
      tar_pos[i] = msg[i].data;
      delta_pos[i] = 0;
    }
      
    for (int j = 0; j < 8; j++)
    {

      ros::spinOnce();
      high_rate.sleep();
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
      }
    }


    double counter = 1;
    cout << endl << "Step Size: " << step_size << endl;
    while (counter < steps)
    {

      msg[1].data = start_pos[1] + (delta_pos[1]*counter*step_size);//Base pitch lead

      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);


      if (msg[3].data < tar_pos[3])
      {
        msg[3].data = start_pos[3] + (10*delta_pos[3]*counter*step_size);//Wrist pitch 
      }
      
      if (action == "Grab")
      {
        msg[3].data = ((-1*M_PI/2) + msg[2].data + msg[1].data);//Wrist pitch
      }
      else if (action == "Place")
      {
        msg[3].data =  msg[2].data + msg[1].data;//Wrist pitch
      }
      msg[4].data = -1*msg[1].data;
      msg[5].data = -1*msg[2].data;
      
      wr_pit_pub.publish(msg[3]);
      ar_pit2_pub.publish(msg[2]);
      ar_pit1_pub.publish(msg[5]);
      
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);

      
      high_rate.sleep();

      ros::spinOnce();
      counter++;
    }

    //Close Gripper
    sleep(2);
    if (action == "Grab")
    {
      msg[7].data = M_PI/3;
      gp_lnr_pub.publish(msg[7]);
    }
    else if (action == "Place")
    {
      msg[7].data = 0;
      gp_lnr_pub.publish(msg[7]);
    }
    loop_rate.sleep();
    ros::spinOnce();
    sleep(1);
  }

  void place_Object()
  {

  }

  //NEED TO ADD RELAX SERVICE TO ACTUALLY RELAX SERVOS (NOT GIPPER AND MAYBE NOT WRIST!)
  void relax_Arm(ros::NodeHandle n,ros::Rate loop_rate,ros::Rate high_rate)
  {
    sleep(3);
    //PUT THIS INIT STUFF IN THE CONSTRUCTOR!!!!
    double tar_pos[8];
    double delta_pos[8];
    double start_pos[8];
    const double steps = 5000;
    double step_size = (1/steps);


    for (int i = 0; i < 8; i++)
    {
      tar_pos[i] = 0;
      delta_pos[i] = 0;
    }

    //Base Pitch
    tar_pos[1] = -1*(M_PI/2);
    tar_pos[4] = -1* tar_pos[1];

    //Arm Pitch
    tar_pos[2] = (M_PI/2);
    tar_pos[5] = -1*tar_pos[2];

    //Wrist pitch
    tar_pos[3] = 0;
    msg[0].data = 0;


    for (int j = 0; j < 8; j++)
    {

        ros::spinOnce();
      high_rate.sleep();
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
      }
    }

    /*
    cout << endl << "Current Position: " << endl;
    cout << endl << "Base: " << start_pos[1] << " " << start_pos[4] << endl;
    cout << endl << "Arm: " << start_pos[2] << " " << start_pos[5] << endl;
    cout << endl << "Base: " << start_pos[3] << endl;

    cout << endl << "Target Position: " << endl;
    cout << endl << "Base: " << tar_pos[1] << " " << tar_pos[4] << endl;
    cout << endl << "Arm: " << tar_pos[2] << " " << tar_pos[5] << endl;
    cout << endl << "Base: " << tar_pos[3] << endl;

    cout << endl << "Delta position: " << endl;
    cout << endl << "Base: " << delta_pos[1] << " " << delta_pos[4] << endl;
    cout << endl << "Arm: " << delta_pos[2] << " " << delta_pos[5] << endl;
    cout << endl << "Base: " << delta_pos[3] << endl;
    */

    double counter = 1;
    cout << endl << "Step Size: " << step_size << endl;
    while (counter < steps)
    {

      
      msg[1].data = start_pos[1] + (delta_pos[1]*counter*step_size);//Base pitch lead

      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);


      //if (msg[3].data < tar_pos[3])
     // {
        msg[3].data = start_pos[3] + (delta_pos[3]*counter*step_size);//Wrist pitch 
     // }
      

      msg[4].data = -1*msg[1].data;
      msg[5].data = -1*msg[2].data;
      
      wr_pit_pub.publish(msg[3]);
      ar_pit2_pub.publish(msg[2]);
      ar_pit1_pub.publish(msg[5]);
      
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);

      
      bs_rot_pub.publish(msg[0]);
      //gp_lnr_pub.publish(msg[7]);
      //wr_rot_pub.publish(msg[6]);
      
      high_rate.sleep();
      ros::spinOnce();
      counter++;
    }
    sleep(1);
  }

  void purgatory_Arm(ros::NodeHandle n,ros::Rate loop_rate,ros::Rate high_rate)
  {
    sleep(3);
    //PUT THIS INIT STUFF IN THE CONSTRUCTOR!!!!
    double tar_pos[8];
    double delta_pos[8];
    double start_pos[8];
    const double steps = 5000;
    double step_size = (1/steps);


    for (int i = 0; i < 8; i++)
    {
      tar_pos[i] = 0;
      delta_pos[i] = 0;
    }

    //Base Pitch
    tar_pos[1] = 0;
    tar_pos[4] = -1* tar_pos[1];

    //Arm Pitch
    tar_pos[2] = 0;
    tar_pos[5] = -1*tar_pos[2];

    //Wrist pitch
    tar_pos[3] = 0;
       msg[3].data = ((-1*M_PI/2) + msg[2].data + msg[1].data);
    for (int j = 0; j < 8; j++)
    {

        ros::spinOnce();
      high_rate.sleep();
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
      }
    }

    /*
    cout << endl << "Current Position: " << endl;
    cout << endl << "Base: " << start_pos[1] << " " << start_pos[4] << endl;
    cout << endl << "Arm: " << start_pos[2] << " " << start_pos[5] << endl;
    cout << endl << "Base: " << start_pos[3] << endl;

    cout << endl << "Target Position: " << endl;
    cout << endl << "Base: " << tar_pos[1] << " " << tar_pos[4] << endl;
    cout << endl << "Arm: " << tar_pos[2] << " " << tar_pos[5] << endl;
    cout << endl << "Base: " << tar_pos[3] << endl;

    cout << endl << "Delta position: " << endl;
    cout << endl << "Base: " << delta_pos[1] << " " << delta_pos[4] << endl;
    cout << endl << "Arm: " << delta_pos[2] << " " << delta_pos[5] << endl;
    cout << endl << "Base: " << delta_pos[3] << endl;
    */

    double counter = 1;
    cout << endl << "Step Size: " << step_size << endl;
    while (counter < steps)
    {

      
      msg[1].data = start_pos[1] + (delta_pos[1]*counter*step_size);//Base pitch lead

      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);


      //if (msg[3].data < tar_pos[3])
      //{
        msg[3].data = start_pos[3] + (delta_pos[3]*counter*step_size);//Wrist pitch 
      //}
      

      msg[4].data = -1*msg[1].data;
      msg[5].data = -1*msg[2].data;
      
      wr_pit_pub.publish(msg[3]);
      ar_pit2_pub.publish(msg[2]);
      ar_pit1_pub.publish(msg[5]);
      
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);
      
      high_rate.sleep();
      ros::spinOnce();
      counter++;
    }
    sleep(1);
  }

