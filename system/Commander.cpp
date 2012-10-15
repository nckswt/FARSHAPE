#include "Commander.h"
#include <fstream>

sensor_msgs::JointState Rx;
sensor_msgs::JointState Tx;
bool send;
bool receive;
bool receiving;
bool sending;

  //Constructor
  Commander::Commander(Position initial_position, std::string robot_name,int argc, char **argv) : FSObject(initial_position, ROBOT_TYPE, robot_name){
		number_of_robots = 3; //default
		if (priority == 1)
			is_master = true;

		if (is_master){
			//use to run roscore, setup, ROS_MASTER_URI etc,
			//<>TODO: implement master_setup.sh
			system("master_setup.sh");
		} else {
			//setup non-master robots
			//<>TODO: implement non_master_setup.sh
			system("non_master_setup.sh");
		}

		//Maybe add code to check other robots for current mission should the robot reset due to power issues

		//ROS SETUP****************************************
		ros::init(argc, argv, name);
		n = new ros::NodeHandle;
		loop_rate = new ros::Rate(10);

		//<>TODO sync with other robots (setting missions, etc)
		helm = new Helm();
		camera = new Camera();
	}

void chatterCallback(const sensor_msgs::JointState::ConstPtr& msg)
{
	ROS_INFO("I Heard: [%s]",msg->name[0].c_str());
	ROS_INFO("I Heard: [%f]",msg->position[0]);
	ROS_INFO("I Heard: [%f]",msg->velocity[0]);
	ROS_INFO("I Heard: [%f]",msg->effort[0]);

	if (receive == true){
		//Possibly use push_back so it stores multiple messages, then cycle through
		receiving = true;
		Rx.name[0] = msg->name[0];
		Rx.position[0] = msg->position[0];
		Rx.velocity[0] = msg->velocity[0];
		Rx.effort[0] = msg->effort[0];
		receiving = false;
	}
}

void Commander::setupComms()
{
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
	Rx.name.push_back("");
	Rx.position.push_back(0.0);
	Rx.velocity.push_back(0.0);
	Rx.effort.push_back(0.0);
	//ros::Publisher pub1 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	//ros::Publisher pub2 =  n->advertise<sensor_msgs::JointState>("Robot_", 20);
	sub = n->subscribe(("Robot_" + ID) , 1000, chatterCallback);
*/}

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
			
			} else if (1/*Make sure the robot stays within its own search zone*/) {
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

	//Communicate with camera, must switch between looking for columns/beams
	/*
		If we locate a bar, do the IR sensors reach it?
			No -> Approach bar
			Yes-> Add to locPieces
		How do we avoid going after the same piece?
	*/

}
void Commander::build()
{
/*	//All robots will have their own structure object, only the master will add to it though, this avoids conflicts
	structure = new Structure(0,0,0);
	ObjectType bar_type;
	int pos = 0;
	bool verify = true;
	pos = structure->next_Piece(&bar_type, &target_x, &target_y, &target_z);
	
	while (pos > 0)//structure is not built
	{
		if (master == false)
		{
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
					//Make sure another robot isnt going to the same location for that piece
					//Or/And just remove it from all robots piece_locations
					//Comms("Remove Piece"...);
					if (1)//either no robots are going for it or it was removed
					{
						//Coordinates from (*it)->getPosition
						//Send coordinates to helm with offset from bar
						//Keep in mind that either the camera or IR sensors need to make sure we are perpendicular
						//and centered
						it = piece_locations.end();//To exit for loop
					}
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

		pos = structure->next_Piece(&bar_type, &target_x, &target_y, &target_z);
	}
*/	
}

void Commander::communicate(std::string cmd, float param1, float param2, float param3)
{
/*	while (sending == true)
	{
		//Careful not to get caught here
	}

	Tx.name.push_back(cmd);
	Tx.position.push_back(param1);
	Tx.velocity.push_back(param2);
	Tx.effort.push_back(param3);

	for (std::vector<ros::Publisher*>::iterator it = transmitters.begin(); it != transmitters.end(); it++)
	{
		if ((*it) != NULL)
		{
			(*it)->publish(Tx);
			//ros::spinOnce();
			//loop_rate->sleep();
		}
	}
	*/
}

void Commander::readCommunications()
{/*
	receive = false;
	localRx.name.push_back(Rx.name[0]);
	localRx.position.push_back(Rx.position[0]);
	localRx.velocity.push_back(Rx.velocity[0]);
	localRx.effort.push_back(Rx.effort[0]);
	receive = true;

	if (localRx.name[0] = "")
	{
		//No command received
	}
	else if (localRx.name[0] = "Ping")
	{
		//Send a return
		//Need to deal with if other robots ping
	}
	else if (localRx.name[0] = "Add Piece")
	{
		//Check to see if this piece has already been found, if not:
		piece_locations.push_back(new FSObject(//ENTER DATA FROM MESSAGE HERE));
	}
	*/
}

void Commander::inspect(){
	//<>TODO: Implement
}


int Commander::checkVitals(){
	//<>TODO: implement
	return 1;
}


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

	Position initial_position{0,0,0,0};
	Commander* commander = new Commander(initial_position, robot_name, argc, argv); //get name from file.

	//Initial setup of comms & create thread to update variables based on ROS messages
	commander->setupComms();

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

	system("PAUSE");//Only for testing purposes, take out before use
	return 0;
}
