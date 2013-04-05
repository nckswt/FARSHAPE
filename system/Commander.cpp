#include "Commander.h"
#include <fstream>
#include <pthread.h>
#include <sstream>


sensor_msgs::JointState f_Rx;
sensor_msgs::JointState f_Tx;
sensor_msgs::JointState localRx;

ros::NodeHandle* n;
ros::Rate* loop_rate;

bool f_send;
bool f_receive;
bool f_receiving;
bool f_sending;

struct passing_thread
{
	int param1;
	int param2;
	char** param3;
	std::string param4;
};



void Commander::demoCommander()
{
	ROS_INFO("Entered demoCommander()");
	//Setup any specific stuff in here
	bool facecheck = false;
	bool pick_up_verify = false;
	float max_range = 40;
	
	//Either start by rotating 180 degrees, or start facing bar
	
	//DO ROTATION HERE IF NECESSARY

	//Possibly give the arm the IR sensor data...if it works well
	//Now we are in position
	//Verify that we picked it up
	while (pick_up_verify == false)//Camera function changes this
	{
		demoSearch(22.0,24.0);
		sleep(2);
		ROS_INFO("Attempting to pick up");
		actuator->actuate_Arm(0.25,0.0,-0.09,"Grab");//For 23 actual we use 25
		sleep(2);
		if (helm->leftRange() > max_range && helm->rightRange() > max_range)
		{
			ROS_INFO("Got the bar!!!");
			pick_up_verify = true;
		}
	}

	//Rotate back to the structure
	demoSearch(29.0,31.0);

	actuator->actuate_Arm(0.26,0.0,0.28,"Top"); //For 30 actual we use 26

	//Now we check to see if the structure is complete
}

//Search for a bar on the ground
void Commander::demoSearch(float min_range, float max_range)
{
	ROS_INFO("Entered demoSearch()");
	//Use camera/IR sensors to lock on to bar
	float left_range = helm->leftRange();
	float right_range = helm->rightRange();
	while (left_range > min_range && left_range < max_range && (abs(left_range - right_range) < 2.0))
	{
		ROS_INFO("In range");
		left_range = helm->leftRange();
		right_range = helm->rightRange();
		if (left_range < kMaxIRRange || right_range < kMaxIRRange)//Add camera tracking 
		{
			ROS_INFO("Attempting to line up");
			helm->lineUp();
		}
		else
		{
			//Move towards using camera, or just go straight some small amount
		}
	}
}

//For a proper demo search
void Commander::demoExplore()
{
	
}

  //Constructor
  Commander::Commander(Position initial_position, std::string robot_name,int argc, char **argv) : FSObject(initial_position, ROBOT_TYPE, robot_name){
		number_of_robots = 3; //default
		if (priority == 1)
			is_master = true;

		/*
		if (is_master){
			//use to run roscore, setup, ROS_MASTER_URI etc,
			//<>TODO: implement master_setup.sh
			system("master_setup.sh");
		} else {
			//setup non-master robots
			//<>TODO: implement non_master_setup.sh
			system("non_master_setup.sh");
		}
		*/
		ROS_INFO("Initializing Commander");
		sleep(1);
		//Maybe add code to check other robots for current mission should the robot reset due to power issues

		//ROS SETUP****************************************
		//ros::init(argc, argv, name);
		//n = new ros::NodeHandle;
		//loop_rate = new ros::Rate(10);

		//<>TODO sync with other robots (setting missions, etc)

		helm = new Helm();
		camera = new Camera();
		actuator = new Arm(argc,argv);
	}

void chatterCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
	ROS_INFO("I Heard: [%s]",msg->name[0].c_str());
	ROS_INFO("I Heard: [%f]",msg->position[0]);
	ROS_INFO("I Heard: [%f]",msg->velocity[0]);
	ROS_INFO("I Heard: [%f]",msg->effort[0]);
	ROS_INFO("I Heard: [%f]",msg->position[1]);

	if (f_receive == true){
		//Possibly use push_back so it stores multiple messages, then cycle through
		f_receiving = true;
		f_Rx.name[0] = msg->name[0];
		f_Rx.position[0] = msg->position[0];
		f_Rx.velocity[0] = msg->velocity[0];
		f_Rx.effort[0] = msg->effort[0];
		f_receiving = false;
	}
}

void Commander::setupComms()//Should be run in the constructor
{
	f_receive = true;
	f_receiving = false;
	f_send = false;
	f_sending = false;
	verify = true;


	f_Rx.name.push_back("NULL");
	f_Rx.name.push_back("NULL");
	f_Rx.position.push_back(0.0);
	f_Rx.velocity.push_back(0.0);
	f_Rx.effort.push_back(0.0);

	f_Tx.name.push_back("NULL");
	f_Tx.name.push_back("NULL");
	f_Tx.position.push_back(0.0);
	f_Tx.velocity.push_back(0.0);
	f_Tx.effort.push_back(0.0);

	localRx.name.push_back("NULL");
	localRx.name.push_back("NULL");
	localRx.position.push_back(0.0);
	localRx.velocity.push_back(0.0);
	localRx.effort.push_back(0.0);
/*	//Set this up to be dynamic
	for (int i = 0; i < number_of_robots - 1; i++)
	{
		if (i == ID)
		{
			transmitters.push_back(NULL);
		}
		else
		{
			//Can we do this?
			transmitters.push_back(new Publisher(n->advertise<sensor_msgs::JointState>("Robot_" + i, 20)));
		}
	}
	f_Rx.name.push_back("");
	f_Rx.position.push_back(0.0);
	f_Rx.velocity.push_back(0.0);
	f_Rx.effort.push_back(0.0);
	//ros::Publisher pub1 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	//ros::Publisher pub2 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	sub = n->subscribe(("Robot_" + ID) , 1000, chatterCallback);
*/}

/*
void Commander::explore()
{
	//<>TODO: change target to an FSObject
	float angle = 0;//Returned from encoders, in Radians
	float left_range, right_range, center_range; //Returned from IR sensors
	int detected_object = 0; //Returns either a beam or a column
	int checkpoints = 0; //search algorithm has 4 points
	angle = helm->getRotation(); //Must be in radians

	//Inital target
	Position target_position;
	target_position.x = position.x + kSearchRadius*cos(angle - (M_PI/6));
	target_position.y = position.y + kSearchRadius*sin(angle - (M_PI/6));

	while(checkpoints < 4){
		//See if object is in view
		//<>TODO: implement!
		//detected_object = camera.detectObject(); // 0 for nothing, 1 for Beam, 2 for Column

		//if object is detected
		if (detected_object != 0){

			//see if object is within range
			left_range = helm->leftRange();
			right_range = helm->rightRange();

			//<>TODO: find center of object;
			//<>STUB: currently just taking the average
			center_range = (left_range + right_range)/2;

			if (left_range < kMaxIRRange || right_range < kMaxIRRange){

				//<>TODO:Check to see if robot is in the way

				//<>TODO check if found object has already been found

				//send position of found object to other robots
				communicate("Add Piece",position.x + center_range*cos(angle),position.y + center_range*sin(angle),detected_object);

				//<>TODO:add object to local piece list
				//piece_locations.push_back(new FSObject(position.x + center_range*cos(center_range),position.y + center_range*sin(center_range),0,detected_object));
				
				//<>TODO: Coordinate piece_locations with other robots
				
				helm->lineUp();


				//<>TODO: Make sure robot will now turn around and continue instead of constantly detecting the same bar!
			
			} else if (1) {
				//<>TODO: If bar is in another search zone, check with the other robots to see if they have found it already,

				//<>TODO: if not, add it
				
				//<>TODO: Head towards the piece based on camera
			}

		} else { //if no object is detected
			
			//<>TODO: Go towards target position
			
			
			if (checkpoints == 3){
			
				//<>TODO: go to center of search area
			
			} else if (fabs(target_position.y - position.y) < 1 && fabs(position.x - target_position.x) < 1){ //close to target

				//<>TODO: rotate 60 degrees, range = radius
				
				checkpoints++;
			}
		}
	}
	//Now share the discovered bars information between robots


}
*/

void Commander::build()
{
	//All robots will have their own structure object, only the master will add to it though, this avoids conflicts
	Position target_position;
	target_position.x = 0;
	target_position.y = 0;
	target_position.z = 0;

	structure = new Structure(target_position);
	ObjectType bar_type;
	int pos = 0;


	pos = structure->nextPiece(&bar_type, &target_position.x, &target_position.y, &target_position.z);
	


	while (pos > 0)//structure is not built
	{
		if (is_master == false)
		{
			communicate("Request Structure",getID(),pos,0,0);
			while(localRx.name[0] != "Approve Structure" && localRx.position[0] != getID() && localRx.velocity[0] != pos)
			{
				//Need some sort of timer, if it fails, ping and check if the master is down!
				readCommunications();
			}
			//check to see if it needs to be verified or not
			//Comms to master to see if this piece should be placed or not
			//if its okay, verify should return true, need to make this member function
			//Also both the master and the slave should mark the piece on the structure as busy
		}
		
		if (verify == true)
		{
			//Go ahead and target the part
			//Use locPieces vector (std::vector<FSObject*> piece_locations;)
			for (std::vector<FSObject*>::iterator it = piece_locations.begin(); it != piece_locations.end(); it++)
			{
				if ((*it)->type == bar_type)
				{
					//Tune these values so it works
					(*it)->getPosition(&target_position);
					target_position.x = target_position.x + 0.1;
					target_position.y = target_position.y + 0.1;
					target_position.z = 0;
					target_position.r = 0;

					helm->goTo(target_position);
					//Coordinates from (*it)->getPosition
					//Send coordinates to helm with offset from bar
					//Keep in mind that either the camera or IR sensors need to make sure we are perpendicular
					//and centered
					it = piece_locations.end();//To exit for loop
				}
				else if (false)//If none are found, search.
				{

				}
			}
			//Make sure another robot isnt going to the same location for that piece
			//Send target to helm, make sure to consider offsets such that we don't run over it
			
			//Orient ourselves perpendicular to the bar and within arm reach
			//Send arm distances
			//Check to see if the piece was grabbed

			verify = false;
		}
		//.....
		pos = structure->nextPiece(&bar_type, &target_position.x, &target_position.y, &target_position.z);
	}
	
}

void Commander::communicate(std::string cmd, float param1, float param2, float param3, float param4)
{
	while (f_sending == true)
	{
		//Careful not to get caught here
		//Maybe add a counter and a sleep
	}

	f_send = false;

	f_Tx.name[0] = cmd;
	f_Tx.position[0] = param1;
	f_Tx.velocity[0] = param2;
	f_Tx.effort[0] = param3;

	f_Tx.position[1] = param4;

	f_send = true;
	/*
	for (std::vector<ros::Publisher*>::iterator it = transmitters.begin(); it != transmitters.end(); it++)
	{
		if ((*it) != NULL)//Double check this, implementation may have changed
		{
			(*it)->publish(f_Tx);
		}
	}
	*/
}

void Commander::readCommunications()
{
	f_receive = false;
	localRx.name.push_back(f_Rx.name[0]);
	localRx.position.push_back(f_Rx.position[0]);
	localRx.velocity.push_back(f_Rx.velocity[0]);
	localRx.effort.push_back(f_Rx.effort[0]);
	f_receive = true;

	if (localRx.name[0] == "NULL")
	{
		//No command received
	}
	else if (localRx.name[0] == "Demo Placed")
	{

	}
	else if (localRx.name[0] == "New Bot")
	{
		//Create new publisher to send to rebooted or added robot
	}
	else if (localRx.name[0] == "Ping")
	{
		//Send a return with coordinates
		Position temp;
		getPosition(&temp);
		communicate("Return",temp.x,temp.y,temp.r,0.0);
	}
	else if (localRx.name[0] == "Return")
	{
		//Used for checking its not a robot or w/e
	}
	else if (localRx.name[0] == "Remove Piece")
	{

	}
	else if (localRx.name[0] == "Add Piece")
	{
		//position -> ID
		//velocity -> type
		//effort -> x
		//position[1] -> y
		//Check to see if this piece has already been found, if not: 
		//piece_locations.push_back(new FSObject(//ENTER DATA FROM MESSAGE HERE));
		for (std::vector<FSObject*>::iterator it = piece_locations.begin(); it != piece_locations.end(); it++)
		{
			if ((*it)->getID() == localRx.position[0])
			{
				it = piece_locations.end();//Should prevent multiple copies of the same message from 
											//creating duplicates.
			}
			else if (it == piece_locations.end())
			{
				Position temp;
				temp.x = localRx.effort[0];
				temp.y = localRx.position[1];
				temp.z = 0;
				temp.r = 0;

				piece_locations.push_back(new FSObject(temp,ObjectType(int(localRx.velocity[0])),"Member"));
				it = piece_locations.end();
			}
		}
		
	}
	else if (is_master == true)
	{
		if (localRx.name[0] == "Request Piece")
		{
			//position[0] is the requestor
			//position[1] is the type
			//velocity is the x
			//effort is the y

			for (std::vector<FSObject*>::iterator it = piece_locations.begin(); it != piece_locations.end(); it++)
			{
				if ((*it)->getType() == int(localRx.position[1]))
				{
					//Go ahead
					//send back to requestor -> position[0] = ID of requestor
					//Send back its okay -> velocity[0] = 1

					communicate("Response Piece",localRx.position[0],1,0,0);

					//Remove from vector
					if (it == piece_locations.end())//Deals with the removal of the last index
					{
						it--;
					}
					piece_locations.erase(it+1);
				}
				else if (it == piece_locations.end())
				{
					//send back to requestor -> position[0] = ID of requestor
					//Send back its no good -> velocity[0] = -1
					communicate("Response Piece",localRx.position[0],-1,0,0);
				}
			}
		}
		else if (localRx.name[0] == "Request Structure")
		{
			//Check if its within the build
			if (localRx.position[0] < structure->getSize())
			{
				if (structure->getStatus(localRx.velocity[0]) == NOT_RESERVED)
				{
					//send back to requestor -> position[0] = ID of requestor
					//send back its okay -> velocity[0] = 1
					communicate("Response Structure",localRx.position[0],1,0,0);
				}
				else if (structure->getStatus(localRx.velocity[0]) == BUSY)
				{
					//send back to requestor -> position[0] = ID of requestor
					//send back its busy -> position[0] = -1
					communicate("Response Structure",localRx.position[0],0,0,0);
				}
				else
				{
					communicate("Response Structure",localRx.position[0],-1,0,0);
				}
			}
		}
	}
	else if (localRx.name[0] == "Response Piece" && localRx.position[0] == getID())
	{
		if (localRx.velocity[0] == 1)
		{
			verify = true;
		}
		else
		{
			verify = false;
		}
	}
	else if (localRx.name[0] == "Response Structure")
	{
		if (localRx.velocity[0] == 1)
		{
			verify = true;
		}
		else
		{
			verify = false;
		}
	}
}


void Commander::inspect(){
	//<>TODO: Implement
}


int Commander::checkVitals(){
	//<>TODO: implement
	return 1;
}

void *thread_function(void *arg)
{
	int numRobots = 3;
	ROS_INFO("Entered Thread");
	//sleep(5);
	struct passing_thread params = *((struct passing_thread*)arg);
	int ID = params.param1;
	ROS_INFO("Params1: [%i]",params.param1);
	//int ID = *((int *)arg);
	
	ros::init(params.param2, params.param3, params.param4);
	n = new ros::NodeHandle;
	loop_rate = new ros::Rate(1);
	//sleep(1);
	//-------------------------------------SETUP--------------------------------------------------------------------
	//ROS Comm Objects
	std::vector<ros::Publisher> transmitters;
	ros::Subscriber subsicle;
	ros::Publisher pub1;
	ros::Publisher pub2;
	ros::Publisher pub3;
	ros::Publisher pubs[3];
	pubs[0] = pub1;
	pubs[1] = pub2;
	pubs[2] = pub3;
	std::string topic = "/Robot_";
	std::ostringstream oss;
	//Set this up to be dynamic

	for (int i = 1; i < numRobots+1; i++)
	{
		ROS_INFO("i: [%i]",i);
		if (i == ID)
		{

		}
		else
		{
			topic = "/Robot_";
			oss << i;
			topic += oss.str();
			ROS_INFO("Publish Topic: [%s]",topic.c_str());
			pubs[i - 1] = n->advertise<sensor_msgs::JointState>(topic, 20);
			transmitters.push_back(pubs[i - 1]);
			oss.str("");
		}
	}

	f_Rx.name.push_back("NULL");
	f_Rx.position.push_back(0.0);
	f_Rx.velocity.push_back(0.0);
	f_Rx.effort.push_back(0.0);

	topic = "/Robot_";
	oss << ID;
	topic += oss.str();
	oss.str("");
	//sleep(1);
	ROS_INFO("Subscribe Topic: [%s]",topic.c_str());
	subsicle = n->subscribe<sensor_msgs::JointState>(topic, 20, chatterCallback);

	//--------------------------------------------------------------------------------------------------------------
	ROS_INFO("Before while loop");
	while (true)
	{
		if (f_send == true)
		{
			for (std::vector<ros::Publisher>::iterator it = transmitters.begin(); it != transmitters.end(); it++)
			{
				(*it).publish(f_Tx);
			}
		}

		loop_rate->sleep();
		ros::spinOnce();
	}
	
	//pthread_exit(NULL);
	return NULL;
}

int main (int argc, char** argv)
{
	ROS_INFO("Main Started.");
	sleep(2);
	Position initial_position{0,0,0,0};
	initial_position.x = 0;
	initial_position.y = 0;
	initial_position.z = 0;
	initial_position.r = 0;
	ROS_INFO("Creating Commander");
	Commander* commander = new Commander(initial_position, "Botty", argc, argv);
	//commander->setupComms();
	sleep(1);
	ROS_INFO("Commander created!");
	commander->demoCommander();
	return 0;
}
/*
int main(int argc, char **argv)
{
	//<>TODO implement init.tab to restart if necessary

	std::ifstream namefile;
	std::string robot_name;
	namefile.open("/etc/hostname");
	if (namefile.is_open())
		getline(namefile,robot_name);
	namefile.close();
	//<>STUB
	std::cout << "Robot name is: " << robot_name << std::endl;

	Position initial_position{0,0,0,0};//Needs to be changed for each robot according to start up positions
	Commander* commander = new Commander(initial_position, robot_name, argc, argv); //get name from file.

	//Initial setup of comms & create thread to update variables based on ROS messages
	commander->setupComms();

	//THREAD CREATION
	struct passing_thread params;
	
	params.param1 = commander.getID();
	params.param2 = argc;
	params.param3 = argv;
	params.param4 = commander.getName();

	pthread_t thread_ID;
	pthread_create(&thread_ID, NULL, &thread_function, &params); 

	std::cout << "well at least we got this far" << std::endl;

	while (!commander->structure_is_complete){
		commander->checkVitals();
		switch(commander->mode){
			case NO_MODE:
				commander->mode = EXPLORER_MODE;
				break;
			case EXPLORER_MODE:
				commander->explore();
				commander->mode = BUILDER_MODE;
				break;
			case BUILDER_MODE:
				commander->build();
				commander->mode = INSPECTOR_MODE;
				break;
			case INSPECTOR_MODE:
				commander->inspect();
				commander->mode = EXPLORER_MODE;
				break;
		}
	}
	return 0;
}
*/