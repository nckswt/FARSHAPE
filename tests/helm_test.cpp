#include "../system/Helm.h"
#include <iostream>
#include <stdlib.h>
#define DISTANCE_TOLERANCE 1
#define ROTATION_TOLERANCE 5

bool checkTolerance( float testValue, int   expectedValue, int tolerance );
bool checkTolerance( int   testValue, int   expectedValue, int tolerance );
void doTest( bool value );
int passedTests = 0;
int failedTests = 0;

int main() {
  
  xyz testLocation;
  float testRotation;
  
  Helm wesley;
  
  std::cout << "Go +40 cm" << std::endl;
  wesley.goDistance(40);
  testLocation = wesley.getLocation();
  doTest ( checkTolerance ( testLocation.y, 40, DISTANCE_TOLERANCE ) );
  doTest ( checkTolerance ( testLocation.x, 0, DISTANCE_TOLERANCE ) );
  
  std::cout << "Go -40 cm" << std::endl;
  wesley.goDistance(-40);
  testLocation = wesley.getLocation();
  doTest ( checkTolerance ( testLocation.y, 0, DISTANCE_TOLERANCE ) );
  doTest ( checkTolerance ( testLocation.x, 0, DISTANCE_TOLERANCE ) );
  
  std::cout << "Rotate +90 degrees" << std::endl;
  wesley.rotate(90);
  testRotation = wesley.getRotation();
  doTest ( checkTolerance ( testRotation, 90, ROTATION_TOLERANCE ) );
  
  std::cout << "Rotate -90 degrees" << std::endl;
  wesley.rotate(-90);
  testRotation = wesley.getRotation();
  doTest ( checkTolerance ( testRotation, 0, ROTATION_TOLERANCE ) );
 
  return 0;
  
}

void doTest ( bool value ) {
  if ( value ) {
    std::cout << "Passed test." << std::endl;
    passedTests++;
  } else {
    std::cout << "Failed test." << std::endl;
    failedTests++;
  }
}

bool checkTolerance ( float testValue, int expectedValue, int tolerance ) {
  
  float relError = abs( expectedValue - testValue ) / abs ( expectedValue );
  
  if ( relError > tolerance ) {
    return false;
  } else {
    return true;
  }
  
}

bool checkTolerance ( int testValue, int expectedValue, int tolerance ) {
  
  float relError = abs( expectedValue - testValue ) / abs ( expectedValue );
  
  if ( relError > tolerance ) {
    return false;
  } else {
    return true;
  }
  
}