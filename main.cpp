#include "system/Commander.h"
#include <pthread.h>

int main(int argc, char const *argv[])
{
	//create the robot commander (the primary controller of the robot)
	Commander commander;

	//Initial setup of comms & create thread to update variables based on ROS messages
	commander.setup_Comms();

	while (!strcture_is_complete){
		switch(commander.mode){
			case NO_MODE:
				commander.checkVitals();
				commander.mode = EXPLORER_MODE;
				break;
			case EXPLORER_MODE:
				commander.explore();
				commander.mode = BUILDER_MODE;
				break;
			case BUILDER_MODE:
				commander.build();
				commander.mode = INSPECTOR_MODE;
				break;
			case INSPECTOR_MODE:
				commander.inspect();
				commander.mode = EXPLORER_MODE;
				break;
		}
	}



	return 0;
}