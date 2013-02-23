#include "../location/encoders.c"

int main() {
  
  // test setting up of encoders
  setupEncoders();
  
  // test getting encoder 1 position
  getEncoderPosition(ENCODER1_ADDRESS);
  
  // test getting encoder 2 position
  getEncoderPosition(ENCODER2_ADDRESS);
  
}