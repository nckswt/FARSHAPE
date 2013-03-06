#include "../system/Navigator.h"
#include "../system/Pilot.h"
#include "../system/i2c.h"
#include "../system/i2cbusses.h"
#include <iostream>
#include <pthread.h>

void* createNav( void* ptr );

Navigator n;

int main () {
  
  pthread_t thread1;

  int iret1;

  char* msg1 = "Thread 1";

  iret1 = pthread_create( &thread1, NULL, createNav, NULL );

  //Pilot p;
  
  //p.setNavigator(&n);
  std::cout << "are we alive? " << std::endl;
  while (1) {
    std::cout << n.getLeft() << std::endl;
    sleep(1);
  }
  
  return 0;
  
}

void* createNav ( void* ptr ) {
  
  n.go();

}
