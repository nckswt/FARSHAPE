#include "FSMember.h"

int FSMember::check_State()
{
	return curState;
}

void FSMember::set_State(int newState)
{
	switch(newState) 
	{
	    case 0: curState = Free;
	    case 1: curState = Busy;
	    case 2: curState = Placed;
	    case 3: curState = Unknown;
	    default: curState = Unknown;
	}
}

int FSMember::get_EntryOrder()
{
	return EntryOrder;
}

void FSMember::set_EntryOrder(int Order)
{
	EntryOrder = Order;
}

int main()
{
	return 0;
}