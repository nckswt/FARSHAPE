#include "../location/IRsensor.h"

int main() {
  ofstream file;
  file.open("sensorCalibrationData");

  for (int i = 0; i < 142; i++){
    cout<< "Sample " << i <<": ";
    file << i << '\t';
    file << readGP2D12ADC() << endl;
    sleep(5);
    }
 file.close();