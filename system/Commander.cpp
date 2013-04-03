#include "Commander.h"
sensor_msgs::JointState rx;


/** Callback function for threaded communication 
 * 
 * Currently only handles position / velocity of other robots ??
 * 
 * @PARAM msg The message 
 */
void chatterCallback(const sensor_msgs::JointState::ConstPtr& msg){
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
 

/** Constructor
*
*	Creates the robot commander.
*
*	@PARAM: intial x,y,z coordinates
*	@PARAM: the robot name
*	@PARAM: command line inputs
*/
Commander::Commander(float xi, float yi, float zi, std::string rName, int argc, char **argv) : FSObject(xi, yi, zi, rName){ //Maybe pull this information from a text file on each robot.
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
    //<>TODO: Create comms object and ping other robots
    //<>TODO: Any other checks performed here
    //<>TODO: Create structure object at (0,0,0)
    
    //<>TODO: Maybe add code to check other robots for current mission should the 
    //      robot reset due to power issues
    
    //ROS SETUP****************************************
    ros::init(argc, argv, Name);
    n = new ros::NodeHandle;
    loop_rate = new ros::Rate(ROS_RATE);
    number_of_robots = 3;//Default
  }

/** Communications set-up
*
*	Initialize ROS communications
*
*/
void Commander::setupComms(){
	//<>TODO: create a thread and return its handle
	//<>TODO: Set this up to be dynamic
	for (int i = 0; i < number_of_robots - 1; i++)
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

/** Explorer Mode
*
*	Go into explorer mode. Search for pieces and pick them up. 
*	Avoid other robots. Stay within boundaries.
*
*/
void Commander::explore(){
	float angle = 0;//Must be in radians
	float range = 0;
	int cam_detect = 0;
	int checkpoints = 0;
	angle = Encoders->get_Angle(); //Must be in radians

	//Inital t
  target_x = position.x + kSearchRadius*cos(angle - (M_PI/6));
  target_y = position.y + kSearchRadius*sin(angle - (M_PI/6));

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
				//First check piece_locations to see if theres an object with a very similar position
				use_Comms("Piece",position.x + range*cos(angle),position.y + range*sin(angle),cam_detect);//z will be used to send if its a beam or a column

				piece_locations.push_back(new FSObject(position.x + range*cos(angle),position.y + range*sin(angle),0,cam_detect));
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
   *		Yes-> Add to piece_locations
   *	How do we avoid going after the same piece?
	*/

}

/** Build Mode
*
*	Go into build mode. Find the next spot to place your beam/column. 
*	Place beam/column.
*
*/
void Commander::build(){
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
			//Use piece_locations vector
			//Send target to helm, make sure to consider offsets such that we don't run over it
			//Orient ourselves perpendicular to the bar and within arm reach
			//Send arm distances
			//Check to see if the piece was grabbed
		}
		//.....

		pos = strctr->next_Piece(&bar_type, &target_x, &target_y, &target_z);
	}
	
}

/** Inspect Mode
*
*	Go into inspection mode. Admire the building work and make sure it's been done correctly. Announce the verification. Announce final construction if possible.
*
*/
void Commander::inspect(){
	//<>TODO: implement!
}

/** Inspect Mode
*
*	Send and receive information from other robots.
*
*	@PARAM: <>CHECK WITH MIKE
*
*/
void Commander::communicate(std::string cmd, float f1, float f2, float f3){
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


/** Read Communications
*
*	Interpret incoming data
*
*/
void Commander::readCommunications(){
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
