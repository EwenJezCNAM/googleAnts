#ifndef LOCATIONINFO_H_
#define LOCATIONINFO_H_

#include "Location.h"
#include <vector>
using namespace std;

struct Node : public Location
{

	int gCost; //Manhattan distance from start
	int hCost; //Manhattan distance from end
	int fCost; //gCost + hCost
	Location start;
	Location target;
	Location current;

private:
	Node* previousNode;
	Node* nextNode;

public:
	void calculateFCost() {
		//f(n) = g(n) + h(n)
		fCost = gCost + hCost;
	};

	void setHCost() {
		hCost = calculateHeuristic(current, target);
	};

	void setGCost() {
		gCost = calculateHeuristic(start, current);
	};

	Node(Location currentNode, Location startNode, Location targetNode)
	{
		start = startNode;
		target = targetNode;
		current = currentNode;
		//previousLocation = nullptr;

		setHCost();
		setGCost();
		calculateFCost();

		//Set row and col
		row = currentNode.row;
		col = currentNode.col;

		//previousLocation = new LocationInfo;
		//nextLocation = new LocationInfo;
	};

	Node()
	{
		fCost = 0;
		hCost = 0;
		gCost = 0;

		//previousLocation = new LocationInfo;
		//nextLocation = new LocationInfo;
	};

	//~LocationInfo()
	//{
	//	delete previousLocation;
	//	delete nextLocation;
	//}


	float calculateHeuristic(const Location a, const Location b) {
		return abs(a.row - b.row) + abs(a.col - b.col); // Manhattan distance because we can only move in 4 directions
	};

	void setPreviousNode(Node& previous)
	{
		previousNode = &previous;
		previousNode->nextNode = this;
	}
	
	Node* getPreviousNode()
	{
		return previousNode;
	}

	Node* getNextNode()
	{
		return nextNode;
	}

};

#endif
;