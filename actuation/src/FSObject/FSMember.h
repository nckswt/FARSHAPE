#ifndef FSMEMBER_H
#define FSMEMBER_H

#include "FSObject.h"

class FSMember: public FSObject
{
private:
	enum State {Free = 0, Busy = 1, Placed = 2, Unknown = 3};
	State curState;
	int EntryOrder;
public:
	FSMember(float xi, float yi, float zi, int _type) : FSObject(xi, yi, zi)
	{
		curState = Free;
		switch(_type)
		{
			case 1: type = Beam;
			case 2: type = Column;
			default: type = Unknown;
		}
	}
	int check_State();
	void set_State(int);
	
	int get_EntryOrder();
	void set_EntryOrder(int);
	~FSMember();
};


#endif