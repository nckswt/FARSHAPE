#include "../location/IRsensor.h"

int main(int argc, char ** argv){
  IRsensor leftIR(0, "../location/left_IR_calibration.txt");
  IRsensor rightIR(1, "../location/right_IR_calibration.txt");

  //output distance
  while (1){
    std::cout << "Left distance: " << leftIR.getDistance() << "cm" << std::endl; 
    std::cout << "Right distance: " << rightIR.getDistance() << "cm" << std::endl; 
    sleep(1);
  }

  return 0;
}
