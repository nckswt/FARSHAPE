#include "../location/IRsensor.h"

int main(int argc, char ** argv){
  IRsensor leftIR(0);
  IRsensor rightIR(1);

  //output distance
  while (1){
    cout << "Left distance: " << leftIR.distanceFromRobot() << "cm" << endl; 
    cout << "Right distance: " << rightIR.distanceFromRobot() << "cm" << endl; 
    sleep(1);
  }

  return 0;
}
