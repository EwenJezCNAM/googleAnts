#ifndef NODE_H_
#define NODE_H_

#include "Location.h"
#include <vector>
using namespace std;

struct Node : public Location
{
public:
	float gCost; //Real distance from start
	float hCost; //Manhattan distance from end
	float fCost; //gCost + hCost



	Node()
	{
		fCost = 0;
		hCost = 0;
		gCost = 0;
	};


	Node* previousNode;

	Node(Node* _previousNode, float _hCost)
	{
		this->previousNode = _previousNode;
		setHCost(_hCost);
		setGCost();
		calculateFCost();

		//Set row and col (pas besoin finalement car l'on hérite de Location)
		//row = currentNode.row;
		//col = currentNode.col;
		

	};

	void calculateFCost() {
		//f(n) = g(n) + h(n)
		fCost = gCost + hCost;
	};

	void setHCost(float _hcost) {
		hCost = _hcost;
	};

	void setGCost() {
		if (previousNode != nullptr) {
			gCost = previousNode->gCost + 1;
		}
		else {
			gCost = 0;
		}

	};

};

#endif
;