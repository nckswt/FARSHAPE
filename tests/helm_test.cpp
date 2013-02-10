#include "../system/Helm.h"
#include <iostream>

int main() {
  
  Helm wesley;
  
  std::cout << "Go +60 cm" << std::endl;
  wesley.goDistance(60);
  
  std::cout << "Go -60 cm" << std::endl;
  wesley.goDistance(-60);
  
  std::cout << "Rotate +90 degrees" << std::endl;
  wesley.rotate(90);
  
  std::cout << "Rotate -90 degrees" << std::endl;
  wesley.rotate(-90);
 
  return 0;
  
}
