#ifndef _PILOT_H_
#define _PILOT_H_

//The pilot executes the mission plan in managable steps by choosing elements from a behaviour list
class Pilot {
private:
  Motor leftMotor;
  Motor rightMotor;
  void _goDistance( float distance );
  // void _rotate( int theta );
  float _getDistance();
  void _resetEncoders();
  Navigator* n;
public:
  void moveTo( float x, float y );
  void rotate ( int theta ); // maybe not needed?
  void setNavigator( Navigator* n );
  Pilot();
  ~Pilot();
};

#endif