#include "../location/encoders.c"
#include <iostream>
#include <stdint.h>
using namespace std;

uint64_t getpos( uint8* posi ) {
  uint64_t pos = 0;
  for ( int i=0; i < 6; i++ ) {
    pos += *(posi+i);
  }
  return pos;
}

int main() {
  
  // test setting up of encoders
  setupEncoders();
  
  // test getting encoder 1 position
  cout << "Encoder 1:" << endl << "Address: " << ENCODER1_ADDRESS << endl << "Position: " << 
  getpos(getEncoderPosition(ENCODER1_ADDRESS)) << endl;
  
  // test getting encoder 2 position
  cout << "Encoder 2:" << endl << "Address: " << ENCODER2_ADDRESS << endl << "Position: " << 
  getpos(getEncoderPosition(ENCODER2_ADDRESS)) << endl;
  
}
