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
  
//   if (this->reversed) {
//     _setValue( (speed / 2) * -1 );
//   } else {
//     _setValue( (speed / 2) );
//   }

  _setValue( (speed/2) * ( this->reversed ? -1 : 1 ) );

}

void Motor::_setValue ( signed int speed ) {
    
    ofstream f;
    f.open(this->ctrl_interface, ios::trunc | ios::out);
    f << int(this->pin) << '=' << speed-150 << "\n";
    f.close();
    
}
