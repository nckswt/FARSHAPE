#include "Commander.h"
sensor_msgs::JointState rx;

/** Callback function for threaded communication 
 * 
 * Currently only handles position / velocity of other robots ??
 * 
 * @PARAM msg The message 
 */
void chatterCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
	ROS_INFO("I Heard: [%s]",msg->name[0].c_str());
	ROS_INFO("I Heard: [%f]",msg->position[0]);
	ROS_INFO("I Heard: [%f]",msg->velocity[0]);
	ROS_INFO("I Heard: [%f]",msg->effort[0]);

	//Possibly use push_back so it stores multiple messages, then cycle through
	rx.name[0] = msg->name[0];
	rx.position[0] = msg->position[0];
	rx.velocity[0] = msg->velocity[0];
	rx.effort[0] = msg->effort[0];
}


void Commander::setup_Comms()
{
	//Set this up to be dynamic
	for (int i = 0; i < numRobots - 1; i++)
	{
		if (i == ID)
		{
			transmitters.push_back(nullptr);
		}
		else
		{
			//Can we do this?
			transmitters.push_back(new Publisher(n->advertise<sensor_msgs::JointState>("Robot_" + i, 20)));
		}
	}
	Rx.name.push_back("");
	Rx.position.push_back(0.0);
	Rx.velocity.push_back(0.0);
	Rx.effort.push_back(0.0);
	//ros::Publisher pub1 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	//ros::Publisher pub2 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	sub = n->subscribe(("Robot_" + ID) , 1000, chatterCallback);
}

void Commander::search_Pieces()
{
	float angle = 0;//Must be in radians
	float range = 0;
	int cam_detect = 0;
	int checkpoints = 0;
	angle = Encoders->get_Angle(); //Must be in radians

	//Inital t
  target_x = position.x + searchRadius*cos(angle - (M_PI/6));
  target_y = position.y + searchRadius*sin(angle - (M_PI/6));

	while(checkpoints < 4)
	{
		cam_detect = check_Camera(); // 0 for nothing, 1 for Beam, 2 for Column
		if (cam_detect != 0)
		{
			//Get -> left or right from camera to guide in, then send to motors
			range = range_IR();
			if (sensorIR->range_IR() < max_range_IR)
			{
				//Check to see if robot is in the way?
				//First check locPieces to see if theres an object with a very similar position
				use_Comms("Piece",position.x + range*cos(angle),position.y + range*sin(angle),cam_detect);//z will be used to send if its a beam or a column

				locPieces.push_back(new FSObject(position.x + range*cos(angle),position.y + range*sin(angle),0,cam_detect));
				//Send this to other robots
				//Make sure robot will now turn around and continue instead of constantly detecting the same bar!
			}
			else if (1/*Make sure the robot stays within its own search zone*/)
			{
				//If bar is in another search zone, check with the other robots to see if they have found it already,
				//if not, then its okay to add
				//Allow camera to guide towards the piece
			}
		}
		else
		{
			//Go towards target
			if (checkpoints == 3)
			{
				//go to center of search area
			}
			else if (position.x == target_x /*within some sort of error*/ && position.y == target_y)
			{
				//rotate 60 degrees, range = radius
				checkpoints++;
			}
		}
	}
	//Now share the discovered bars information between robots

	//Communicate with camera, must switch between looking for columns/beams
	/*
   *	If we locate a bar, do the IR sensors reach it?
   *		No -> Approach bar
   *		Yes-> Add to locPieces
   *	How do we avoid going after the same piece?
	*/

}
void Commander::build_Structure()
{
	//All robots will have their own structure object, only the master will add to it though, this avoids conflicts
	strctr = new Structure(0,0,0);
	int bar_type = 0;
	int pos = 0;
	bool verify = true;
	pos = strctr->next_Piece(&bar_type, &target_x, &target_y, &target_z);
	
	while (pos > 0/*structure is not built*/)
	{
		if (master == false)
		{
			//check to see if it needs to be verified or not
			//Comms to master to see if this piece should be placed or not
			//if its okay, verify should return true, need to make this member function
		}
		
		if (verify == true)
		{
			//Go ahead and target the part
			//Use locPieces vector
			//Send target to helm, make sure to consider offsets such that we don't run over it
			//Orient ourselves perpendicular to the bar and within arm reach
			//Send arm distances
			//Check to see if the piece was grabbed
		}
		//.....

		pos = strctr->next_Piece(&bar_type, &target_x, &target_y, &target_z);
	}
	
}

void Commander::use_Comms(std::string cmd, float f1, float f2, float f3)
{
	Tx.name.push_back(cmd);
	Tx.position.push_back(f1);
	Tx.velocity.push_back(f2);
	Tx.effort.push_back(f3);

	for (std::vector<ros::Publisher*>::iterator it = transmitters.begin(); it != transmitters.end(); it++)
	{
		if ((*it) != nullptr)
		{
			(*it)->publish(Tx);
			ros::spinOnce();
			loop_rate->sleep();
		}
	}

	ros::spinOnce();//Insert this line in all loops...
	loop_rate->sleep();
}

void Commander::check_Comms()
{
	if (rx.name[0] = "")
	{

	}
	else if (rx.name[0] = "Ping")
	{

	}
	else if (rx.name[0] = "Piece")
	{

	}
}
