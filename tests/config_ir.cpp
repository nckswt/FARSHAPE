#include "../location/IRsensor.h"

int main() {
  
  IRsensor sens( 0 );
  
  ofstream file;
  file.open("sensorCalibrationData");

  for (int i = 0; i < 142; i++){
    cout<< "Sample " << i <<": ";
    file << i << '\t';
    file << sens.getValue() << endl;
    sleep(5);
    }
  file.close();
 
  return 0;
}