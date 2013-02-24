#include "../location/encoders.c"
#include <iostream>
using namespace std;

int main() {
  
  // test setting up of encoders
  setupEncoders();
  
  // test getting encoder 1 position
  cout << "Encoder 1:" << endl << "Address: " << ENCODER1_ADDRESS << endl << "Position: " << getEncoderPosition(ENCODER1_ADDRESS) << endl;
  
  // test getting encoder 2 position
  cout << "Encoder 2:" << endl << "Address: " << ENCODER2_ADDRESS << endl << "Position: " << getEncoderPosition(ENCODER2_ADDRESS) << endl;
  
}
