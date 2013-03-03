#ifndef _NAVIGATOR_H_
#define _NAVIGATOR_H_

#include <string>
#include <stdint.h>
#include "../location/Encoder.h"

struct SphCoord{
  double r; //distance to object
  double theta; //lateral angle
  double phi; //elevation angle
};

struct Cart3DCoord{
  double x;
  double y;
  double z;
};

struct ZoneEdges{
  double north;
  double south;
  double east;
  double west;
};

class Navigator{
public:
  Navigator();
  ~Navigator();

  // image stuff

  // not applicable:
  // void identifyTarget(std::string imageFile);
  // Cart3DCoord* identifyObjects();
  // SphCoord cart2spherical(Cart3DCoord position);
  // Cart3DCoord getTargetPosition();	


private:
  
  // natural members of the object
  Cart3DCoord currentPosition; 
  Cart3DCoord structurePosition();
  Cart3DCoord* memberPositions;
  
  // member encoders
  Encoder leftEncoder;
  Encoder rightEncoder;	
  uint64_t leftEncoderPos;
  uint64_t rightEncoderPos;

  // not implemented
  /*ZoneEdges mapZoneEdges();
  ZoneEdges zoneEdges;
  Cart3DCoord* otherRobotPositions;
  Cart3DCoord targetPosition;
*/
};

#endif