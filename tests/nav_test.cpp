#include "../system/Navigator.h"
#include <iostream>

int main () {
  
  Navigator n;
  
  n = Navigator();
  
  while (1) {
    std::cout<< n.getLeft() << std::endl;
    sleep(1);
  }
  
}