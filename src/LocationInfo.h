#ifndef LOCATIONINFO_H_
#define LOCATIONINFO_H_

#include "Location.h"
#include <vector>
using namespace std;

struct LocationInfo : public Location
{

	int gCost; //Manhattan distance from start
	int hCost; //Manhattan distance from end
	int fCost; //gCost + hCost
	Location start;
	Location target;
	Location current;

private:
	LocationInfo* previousLocation;
	LocationInfo* nextLocation;

public:
	LocationInfo(Location currentLocation, Location startLocation, Location targetLocation)
	{
		start = startLocation;
		target = targetLocation;
		current = currentLocation;
		//previousLocation = nullptr;

		setHCost();
		setGCost(gCost);
		calculateFCost();

		//Set row and col
		row = currentLocation.row;
		col = currentLocation.col;

		/*previousLocation = new LocationInfo();
		nextLocation = new LocationInfo();*/
	};

	LocationInfo()
	{
		fCost = 0;
		hCost = 0;
		gCost = 0;

		/*previousLocation = new LocationInfo();
		nextLocation = new LocationInfo();*/
	};

	/*~LocationInfo()
	{
		delete previousLocation;
		delete nextLocation;
	}*/


	void calculateFCost() {
		//f(n) = g(n) + h(n)
		fCost = gCost + hCost;
	};

	void setHCost() {
		hCost = calculateHeuristic(current, target);
	};

	void setGCost(int cost) {
		gCost = calculateHeuristic(start, current);
	};

	float calculateHeuristic(const Location a, const Location b) {
		return abs(a.row - b.row) + abs(a.col - b.col); // Manhattan distance because we can only move in 4 directions
	};

	void setPreviousLocation(AstarClass previous)
	{
		if (previous.getWeight() < previousLocation->getWeight())
		previousLocation = &previous;
		previousLocation->nextLocation = this;
	}
	
	LocationInfo* getPreviousLocation()
	{
		return previousLocation;
	}
	
	LocationInfo* getNextLocation()
	{
		return nextLocation;
	}

};

#endif
;