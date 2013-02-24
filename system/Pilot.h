
#include "Behaviour.h"
#include <vector>

//The pilot executes the mission plan in managable steps by choosing elements from a behaviour list
class Pilot
{
private:
	std::Vector<Behaviour> plan;
	bool releaseCondition;
	bool planIsSetup;

public:
	Pilot();
	~Pilot();
	void executePlan();
	void updatePlan();
	bool checkReleaseCondition();
};

