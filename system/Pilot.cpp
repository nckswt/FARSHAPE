#include "Pilot.h"
#include "../location/Encoder.h"
#include "Behaviour.h"
#include "../mobility/Motors.cpp"

volatile Pilot pilot();
volatile Motors motors();

Pilot::Pilot(){ 
}

Pilot::~Pilot(){

}

/*void Pilot::executePlan(){
  this->releaseCondition = 1;
  while (checkReleaseCondition()){
    if (!this->planIsSetup)
      this->updatePlan();

    //<>TODO: REMOVE STUB!----------------------
    behaviours.trackTarget();
    //------------------------------------------
    // for(std::vector<Behaviours>::iterator it = plan.begin(); it != plan.end(); ++it) {
    // 	}
  }

}*/

/*void Pilot::updatePlan(){
	//set plan to track object

	//<>TODO: REMOVE STUB!----------------------
	behaviours.setupActuatorsAndSensors();
	//------------------------------------------
	this->planIsSetup = 1;
}*/

/*bool Pilot::checkReleaseCondition(){
	//<>TODO: REMOVE STUB!----------------------
	return 1;
	//------------------------------------------
}*/