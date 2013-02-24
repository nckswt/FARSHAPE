Pilot::Pilot(){

}

Pilot::~Pilot(){

}

void Pilot::executePlan(){
	this.releaseCondition = 1;
	while (checkReleaseCondition());
		if (!this.planIsSetup)
			this.setUpPlan();

		for(std::vector<Behaviour>::iterator it = plan.begin(); it != plan.end(); ++it) {
			//execute function
			}

}

void Pilot::updatePlan(){
	//set plan to track object
}

bool Pilot::checkReleaseCondition(){
	//

}