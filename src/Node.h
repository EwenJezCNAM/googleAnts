/*
/!\ THIS CLASS IS NOT USED FOR THE BOT /!\

Class Node which is used for the A* algorithm

*/


#ifndef NODE_H_
#define NODE_H_

#include "Location.h"
#include <vector>
using namespace std;

struct Node : public Location
{
public:
	float m_gCost; //Real distance from start
	float m_hCost; //Manhattan distance from end
	float m_fCost; //gCost + hCost

	Node* mp_previousNode;

	Node()
	{
		m_fCost = 0;
		m_hCost = 0;
		m_gCost = 0;
	};

	Node(Node* _previousNode, float _hCost)
	{
		this->mp_previousNode = _previousNode;
		setHCost(_hCost);
		setGCost();
		calculateFCost();
	};

	void calculateFCost() {
		//f(n) = g(n) + h(n)
		m_fCost = m_gCost + m_hCost;
	};

	void setHCost(float _hcost) {
		m_hCost = _hcost;
	};

	void setGCost() {
		if (mp_previousNode != nullptr) {
			m_gCost = mp_previousNode->m_gCost + 1;
		}
		else {
			m_gCost = 0;
		}

	};

};

#endif
;