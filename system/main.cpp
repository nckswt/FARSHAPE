#include "Pilot.h"
#include "Navigator.h"
#include "Behaviours.h"

int main(int argc, char const *argv[])
{
	Pilot pilot;
	Navigator navigator;

	pilot.executePlan();
	
	return 0;
}