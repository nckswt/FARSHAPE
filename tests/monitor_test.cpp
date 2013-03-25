#include "../power/Monitor.h"
#include <iostream>

int main() {
  
  Monitor bus_5v(   V5_BUS, mv_320 );
  Monitor bus_9v(   V9_BUS, mv_40  );
  Monitor bus_11v( V11_BUS, mv_40  );
  
  std::cout << bus_5v.getVoltage() << std::endl;
  std::cout << bus_9v.getVoltage() << std::endl;
  std::cout << bus_11v.getVoltage() << std::endl;
  
  return 0;
  
}