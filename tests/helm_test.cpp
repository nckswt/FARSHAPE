#include "../system/Helm.h"
#include <iostream>

int main() {
  
  Helm wesley;
  
  std::cout << "Go +40 cm" << std::endl;
  wesley.goDistance(40);
  
  std::cout << "Go -40 cm" << std::endl;
  wesley.goDistance(-40);
  
  std::cout << "Rotate +90 degrees" << std::endl;
  wesley.rotate(90);
  
  std::cout << "Rotate -90 degrees" << std::endl;
  wesley.rotate(-90);
 
  return 0;
  
}
