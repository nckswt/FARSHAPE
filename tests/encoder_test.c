#include "../location/Encoder.h"
#include <iostream>
#include <stdint.h>
#include <time.h>
using namespace std;

int main() {
  
  Encoder* enc; // holds encoder object
  uint64_t p; // holds position
  
  // test setting up of encoder1
  enc = new Encoder ( ENCODER1_ADDRESS , false );
    
  // test getting encoder 1 position
  p = enc->getEncoderPosition();
  cout << "Encoder 1:" << endl << "Address: " << ENCODER1_ADDRESS << endl << "Position: " << 
  p << endl;

    // test setting up of encoder 2
  enc = new Encoder ( ENCODER2_ADDRESS , true );
    
  // test getting encoder 2 position
  p = enc->getEncoderPosition();
  cout << "Encoder 2:" << endl << "Address: " << ENCODER1_ADDRESS << endl << "Position: " << 
  p << endl;
  
}
