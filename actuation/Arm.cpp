#include "Arm.h"
double servo_pos[8];
using namespace std;

void servoCallBack(const sensor_msgs::JointState::ConstPtr& msg)
{
  	servo_pos[0] = msg->position[4];  //Base rotate
    servo_pos[1] = msg->position[6];  //Base pitch lead
    servo_pos[2] = msg->position[3];  //Arm pitch lead
    servo_pos[3] = msg->position[0];  //Wrist pitch
    servo_pos[4] = msg->position[5];  //Base pitch follow
    servo_pos[5] = msg->position[2];  //Arm pitch follow
    servo_pos[6] = msg->position[7];  //Wrist rotate
    servo_pos[7] = msg->position[1];  //Gripper

    //cout << msg->position[6] << " " << servo_pos[1] << endl;
}


Arm::Arm(int argc, char **argv)
{
	//ROS SETUP****************************************
	ros::init(argc, argv, "Arm");
	arm_handle = new ros::NodeHandle;
	loop_rate = new ros::Rate(100);
	high_rate = new ros::Rate(1000);

	wr_rot_pub = arm_handle->advertise<std_msgs::Float64>("/wrist_rotate/command/", 20);
  bs_rot_pub = arm_handle->advertise<std_msgs::Float64>("/base_rotate/command/", 20);
  bs_pit1_pub = arm_handle->advertise<std_msgs::Float64>("/base_pitch1/command/", 20);
  bs_pit2_pub = arm_handle->advertise<std_msgs::Float64>("/base_pitch2/command/", 20);
  ar_pit1_pub = arm_handle->advertise<std_msgs::Float64>("/arm_pitch1/command/", 20);
  ar_pit2_pub = arm_handle->advertise<std_msgs::Float64>("/arm_pitch2/command/", 20);
  wr_pit_pub = arm_handle->advertise<std_msgs::Float64>("/wrist_pitch/command/", 20);
  gp_lnr_pub = arm_handle->advertise<std_msgs::Float64>("/gripper_width/command/", 20);
  servo_feedback = arm_handle->subscribe<sensor_msgs::JointState>("/joint_states", 20, servoCallBack);


  //KDL CHAIN SETUP************************************
  chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotZ),KDL::Frame(KDL::Rotation::RPY(0,0,0),KDL::Vector(0.0,0.0,0.04))));
	chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0.0,0.0),KDL::Vector(0,0.0,0.15))));
	chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::None),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.02,0.0,0))));
	chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.16,0.0,0.0))));
	chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotY),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.13,0.0,0))));
	chain1.addSegment(KDL::Segment(KDL::Joint(KDL::Joint::RotX),KDL::Frame(KDL::Rotation::RPY(0,0,0.0),KDL::Vector(0.0,0.0,0))));

	q = new KDL::JntArray(chain1.getNrOfJoints());
	q_init = new KDL::JntArray(chain1.getNrOfJoints());

	fksolver1 = new KDL::ChainFkSolverPos_recursive(chain1);
  iksolver1v = new KDL::ChainIkSolverVel_pinv(chain1);
	iksolver1 = new KDL::ChainIkSolverPos_NR(chain1,*fksolver1,*iksolver1v,100,1e-6);

  //Added this code!
  //May need to remove if it no longer works...
  ros::spinOnce();
  sleep(2);
}


int Arm::interact_Arm(double x, double y, double z, string action)
{
	double tar_pos[8];
    double delta_pos[8];
    double start_pos[8];
    const double steps = 2500;
    double step_size = (1/steps);
    double counter = 1;

	sleep(3);
    
    //Set destination frame
    double r = sqrt((x*x) + (y*y));
    double rotation = 0;

    //Set parameters depending on action
    if (action == "Grab")
    {
      rotation = (90*M_PI/180);
      msg[6].data = 0;//Wrist rotate
      msg[7].data = 0;//Gripper open
    }
    else if (action == "Top")
    {
      rotation = 0.0;
      msg[6].data = 0;//Wrist rotate
      msg[7].data = M_PI/3;//Gripper close
    }
    else if (action == "Bottom")
    {
      rotation = (90*M_PI/180);
      msg[6].data = 0;//Wrist rotate
      msg[7].data = M_PI/3; //Gripper close
    }
    else if (action == "Column")
    {
      rotation = 0;
      msg[6].data = M_PI/2;//Wrist rotate
      msg[7].data = M_PI/3;//Gripper close
    }

    F_dest = KDL::Frame(KDL::Rotation::RPY(0, rotation, 0), KDL::Vector(r, 0.0, z));
    //F_dest = KDL::Frame(KDL::Rotation::RPY(0, rotation, 0), KDL::Vector(r, 0.0, z));
    //Solve and check to see if it failed
    int ret = iksolver1->CartToJnt(*q_init, F_dest, *q);
    
    if (ret < 0)
    {
      cout << ret << endl;
 		 return -1;//Error occured during IK
 	  }

    for (int i = 0; i < chain1.getNrOfJoints(); i++)
    {
      q_init->data[i] = 0.01;//LOAD THE LAST POSITION HERE
    }

    for (int j = 0; j < (q->rows() - 1); j++) 
    {
      msg[j].data = q->data[j];

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

    msg[0].data = atan2(y,x);//Orient according to angle
    bs_rot_pub.publish(msg[0]);
    gp_lnr_pub.publish(msg[7]);
    wr_rot_pub.publish(msg[6]);

    //Set target
    for (int i = 0; i < 8; i++)
    {
      tar_pos[i] = msg[i].data;
      delta_pos[i] = 0;
    }
    
    tar_pos[1] = tar_pos[1];
    tar_pos[3] = -1*tar_pos[3];
    //Get servo positions and calculate rotational distance needed
    for (int j = 0; j < 8; j++)
    {
      ros::spinOnce();
      high_rate->sleep();
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
      }
    }

    while (counter < steps)
    {

      msg[1].data = start_pos[1] + (delta_pos[1]*counter*step_size);//Base pitch lead

      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);


    
      if (action == "Grab")
      {
        msg[3].data = (-1*(M_PI/2) + msg[2].data + msg[1].data);//Wrist pitch
      }
      else if (action == "Top")
      {
        msg[3].data =  (msg[2].data + msg[1].data);//Wrist pitch
      }
      else if (action == "Bottom")
      {
        msg[3].data = (-1*(M_PI/2) + msg[2].data + msg[1].data);//Wrist pitch
      }
      else if (action == "Column")
      {
        msg[3].data =  (msg[2].data + msg[1].data + 0.15);//Wrist pitch
      }

      msg[4].data = -1*msg[1].data;
      msg[5].data = -1*msg[2].data;
      
      wr_pit_pub.publish(msg[3]);
      ar_pit2_pub.publish(msg[2]);
      ar_pit1_pub.publish(msg[5]);
      
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);

      
      high_rate->sleep();

      ros::spinOnce();
      counter++;
    }

    
    sleep(2);
    if (action == "Grab")
    {
      //Close Gripper
      msg[7].data = M_PI/3;
      gp_lnr_pub.publish(msg[7]);
    }
    else if (action == "Top")
    {
      //Open Gripper
      msg[7].data = 0;
      gp_lnr_pub.publish(msg[7]);
    }
    else if (action == "Bottom")
    {
      msg[7].data = 0;
      gp_lnr_pub.publish(msg[7]);
    }
    else if (action == "Column")
    {
      msg[7].data = 0;
      gp_lnr_pub.publish(msg[7]);
    }

    loop_rate->sleep();
    ros::spinOnce();
    sleep(4);
    return 1;
}


  //NEED TO ADD RELAX SERVICE TO ACTUALLY RELAX SERVOS (NOT GIPPER AND MAYBE NOT WRIST!)
  void Arm::relax_Arm()
  {
    sleep(2);
    //PUT THIS INIT STUFF IN THE CONSTRUCTOR!!!!
    double tar_pos[8];
    double delta_pos[8];
    double start_pos[8];
    const double steps = 2500;
    double step_size = (1/steps);
    double counter = 1;

    for (int i = 0; i < 8; i++)
    {
      tar_pos[i] = 0;
      delta_pos[i] = 0;
    }

    //Base Pitch
    tar_pos[1] = -1*(M_PI/2);
    tar_pos[4] = -1*tar_pos[1];

    //Arm Pitch
    tar_pos[2] = (M_PI/2);
    tar_pos[5] = -1*tar_pos[2];

    //Wrist pitch
    tar_pos[3] = 0;
    msg[0].data = 0;


    for (int j = 0; j < 8; j++)
    {

        ros::spinOnce();
      high_rate->sleep();
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
      }
    }

    while (counter < steps)
    {
      ros::spinOnce();
      
      if ((msg[1].data < tar_pos[1] && delta_pos[1] > 0) || (msg[1].data > tar_pos[1] && delta_pos[1] < 0))
      {
        msg[1].data = start_pos[1] + (100*delta_pos[1]*counter*step_size);//Base pitch lead
        //cout << "ACCELERATED" << endl;
      }

      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);//Arm pitch lead
	   
      if (counter < steps/25)
      {
        msg[3].data = msg[1].data;
      }
      else
      {
        msg[3].data = start_pos[3] + (delta_pos[3]*counter*step_size);//Wrist pitch 
      }


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
      
      high_rate->sleep();
      ros::spinOnce();
      counter++;
    }
    sleep(1);
  }

  void Arm::purgatory_Arm()
  {
    sleep(1);
    float tar_pos[8];
    float delta_pos[8];
    float start_pos[8];
    const double steps = 2000;
    float step_size = (1/steps);
    float counter = 1;

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


    for (int j = 0; j < 400; j++)
    {
      bs_rot_pub.publish(msg[0]);
      for (int i = 0; i < 8; i++)
      {
        delta_pos[i] = tar_pos[i] - servo_pos[i];
        start_pos[i] = servo_pos[i];
        ros::spinOnce();
        loop_rate->sleep();
      }  
    }

    while (counter < steps)
    {
      msg[1].data = start_pos[1] + (delta_pos[1]*counter*step_size);//Base pitch lead
      msg[2].data = start_pos[2] + (delta_pos[2]*counter*step_size);//Arm pitch lead
      msg[3].data = start_pos[3] + (delta_pos[3]*counter*step_size);//Wrist pitch
      
      //DEBUGGING CODE.......................................
      /*
      cout << endl << "Delta: " << delta_pos[1] << endl;
      cout << "Servo Pos: " << servo_pos[1] << endl;
      cout << "Base Pitch" << endl;
      cout << msg[1].data << endl;
      cout << msg[4].data << endl;
      */
      //....................................................

      msg[4].data = -1*msg[1].data;
      msg[5].data = -1*msg[2].data;
      
      wr_pit_pub.publish(msg[3]);
      ar_pit2_pub.publish(msg[2]);
      ar_pit1_pub.publish(msg[5]);
      
      bs_pit1_pub.publish(msg[1]);
      bs_pit2_pub.publish(msg[4]);
      
      high_rate->sleep();
      ros::spinOnce();
      counter++;
    }
    sleep(1);
  }

  int Arm::actuate_Arm(double x, double y, double z, string action)
  {

  	this->purgatory_Arm();
  	int ret = this->interact_Arm(x,y,z,action);
  	this->relax_Arm();
  	return ret;
  }

/*
  int main(int argc, char **argv)
  {
  	Arm* Actuator = new Arm(argc,argv);
  	Actuator->actuate_Arm(0.25,0.0,-0.09,"Grab");//For 23 actual we use 25
    Actuator->actuate_Arm(0.31,0.0,0.06,"Column");
    //Actuator->actuate_Arm(0.26,0.0,0.28,"Top"); //For 30 actual we use 26
  	return 0;
  }
  */