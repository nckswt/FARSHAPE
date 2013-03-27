#include "../system/ADC_SPI.h"
#include <iostream>

int main() {
  
  ADC_SPI adc(0, 0);
  
  while (1) {
    std::cout << adc.getSingle() << std::endl;
    sleep(1);
  }
  
  return 0;
}
