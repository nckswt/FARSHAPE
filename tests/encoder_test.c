#include "../location/Encoder.h"
#include "../system/i2c.h"
#include <iostream>
#include <stdint.h>
#include <time.h>
using namespace std;


int main() {
  
  Encoder enc; // holds encoder object
  uint64_t p; // holds position
  
  // test setting up of encoder1
  enc = Encoder ( ENCODER1_ADDRESS , false, false );
  cout << "Encoder 1 address: " << enc.getAddress() << endl;
  
  // test setting up of encoder 2
  Encoder enc2 = Encoder ( ENCODER2_ADDRESS , true, true );
  cout << "Encoder 2 address: " << enc2.getAddress() << endl;
  
  while (1) {
    // test getting encoder 1 position
    p = enc.getPosition();
    cout << "Encoder 1 Position: " << p << endl;
    // test getting encoder 2 position
    p = enc2.getPosition();
    cout << "Encoder 2 Position: " << p << endl;  
  
    sleep(1);
    
  }
}
