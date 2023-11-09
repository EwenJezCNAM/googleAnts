#ifndef NEIGHBORINFO_H_
#define NEIGHBORINFO_H_

#include "Location.h"

struct NeighborInfo
{
	Location neighborLocation; // start neighbor position
	int distanceToTarget; // distance from the neighbor to the goal

	NeighborInfo(Location loc = Location(), int distance = 0)
	{
		neighborLocation = loc;
		distanceToTarget = distance;
	}

};

#endif;