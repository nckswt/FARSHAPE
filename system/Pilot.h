#ifndef _PILOT_H_
#define _PILOT_H_

#include "Behaviours.h"
#include <vector>

//The pilot executes the mission plan in managable steps by choosing elements from a behaviour list
class Pilot{
private:
	//std::Vector<Behaviour> plan;
	bool releaseCondition;
	bool planIsSetup;
	Behaviours behaviours;

public:
	Pilot();
	~Pilot();
	void executePlan();
	void updatePlan();
	bool checkReleaseCondition();
};

#endif