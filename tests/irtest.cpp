#include "../location/IRsensor.h"

int main(int argc, char ** argv){
  IRsensor leftIR(0);
  IRsensor rightIR(1);

  //output distance
  while (1){
    cout << "Left distance: " << leftIR.getDistance() << "cm" << endl; 
    cout << "Right distance: " << rightIR.getDistance() << "cm" << endl; 
    sleep(1);
  }

  return 0;
}
