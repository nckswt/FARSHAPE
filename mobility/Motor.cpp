#include "Motor.h"
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

Motor::Motor() {
  
}

Motor::Motor( uint8_t pin, bool reversed, char* ctrl_interface ) {
  
  this->pin = pin;
  this->reversed = reversed;
  this->ctrl_interface = ctrl_interface;
  
}

Motor::~Motor() {
}

void Motor::setSpeed( int speed ) {
  
  if ((speed > 100) || (speed < -100)) { 
    // TODO: handle error
    return;
  }

  currentSpeed = speed;

  _setValue( ((speed/2) * ( this->reversed ? -1 : 1 )) );

}

void Motor::_setValue ( signed int speed ) {
    if (speed == 0)
        speed = 130;
    cout << int(this->pin) << '=' << 150-speed << endl;
    ofstream f;
    f.open(this->ctrl_interface, ios::trunc | ios::out);
    f << int(this->pin) << '=' << 150-speed << "\n";
    f.close();
    
}

int Motor::getSpeed() {
  
  return int(currentSpeed);
  
}

void Motor::brake() {
  
  _setValue( 0 );
  
}
