#ifndef NODE_H_
#define NODE_H_

#include "Location.h"
#include <vector>
using namespace std;

struct Node : public Location
{

	int gCost; //Real distance from start
	int hCost; //Manhattan distance from end
	int fCost; //gCost + hCost


private:
	Node()
	{
		fCost = 0;
		hCost = 0;
		gCost = 0;
	};


public:

	Node* previousNode;

	Node(Node* _previousNode, int _hCost)
	{

		setHCost(_hCost);
		setGCost();
		calculateFCost();

		//Set row and col (pas besoin finalement car l'on hérite de Location)
		//row = currentNode.row;
		//col = currentNode.col;
		this->previousNode = _previousNode;

	};

	void calculateFCost() {
		//f(n) = g(n) + h(n)
		fCost = gCost + hCost;
	};

	void setHCost(int _hcost) {
		hCost = _hcost;
	};

	void setGCost() {
		gCost = previousNode->gCost + 1;
	};

};

#endif
;