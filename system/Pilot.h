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

public:
  Pilot(); // should set up actuators and sensors
  ~Pilot();
  void executePlan();
  void updatePlan();
  bool checkReleaseCondition();
  void goDistance(double targetDistance, int speed);
  void rotateAngle(double targetAngle, int speed);
  void moveToTarget(SphCoord targetPosition);
  void grabTarget(Cart3DCoord targetPosition);
};

#endif