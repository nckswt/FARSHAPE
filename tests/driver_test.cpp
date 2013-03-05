#include "../system/Navigator.h"
#include "../system/Pilot.h"
#include "../system/i2c.h"
#include "../system/i2cbusses.h"
#include <iostream>

int main () {
  
  Navigator n;
  
  //Pilot p;
  
  //p.setNavigator(&n);
  
  while (1) {
    std::cout << n.getLeft() << std::endl;
  }
  
  return 0;
  
}