#ifndef FSOBJECT_H
#define FSOBJECT_H

#include <string>
#include <iostream>
#include "includes.h"

/// Defines the types used
enum ObjectType { UNKNOWN, ROBOT, BEAM, COLUMN, STRUCTURE };

class FSObject
{
protected:
  int ID;
  static int nextID;
  std::string Name;
  
  enum ObjectType type;
  
  xyzr position;
  float length, width, height;
  
public: 
  FSObject(float,float,float,int,std::string name = "");
  void get_Position(float* tx, float* ty, float* tz);
  void set_Position(float,float,float);
  int get_NextID();
  void reset_ID(int);
  int	get_ID();
  //~FSObject();
};


#endif