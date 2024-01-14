#include "Astar.h"
#include "Node.h"
#include "State.h"
#include "Location.h"
#include <iostream>
#include <stdexcept>


int MAX_ITERATION = 100;

Location Astar::getNextMove(Location start, Location target, State& state)
{
    state.bug << "La fonction a commence" << endl;

    // Use A* search algorithm for every neighbors of the ant.
    // Then, we move the ant on the neighbor according the shortest path found

    vector<Node> nodeToEvaluate = vector<Node>();//the set of node to be evaluated
    vector<Node> nodeEvaluated = vector<Node>();//set of node already evaluated

    float hcost = calculateHeuristic(start, target);
    state.bug << "hcost : " << hcost << endl;

    Node currentNode = Node(nullptr, hcost);
    currentNode.row = start.row;
    currentNode.col = start.col;
    state.bug << "current node cree" << endl;

    nodeToEvaluate.push_back(currentNode);

    state.bug << "La boucle while commence" << endl;

    //while the set of node to be evaluated is not empty
    while (!nodeToEvaluate.empty())
    {
        // Get the next node to test (the node with the lowest f cost)
        nodeToEvaluate = sort(nodeToEvaluate);
        currentNode = nodeToEvaluate[0];
        //for (int i = 0; i < nodeToEvaluate.size(); i++)
        //{
        //    state.bug << "fCost [" << i << "] : " << nodeToEvaluate[i].fCost << endl;
        //}


        state.bug << "currentNode, x=" << currentNode.row << "y=" << currentNode.col << endl;




        // Check if the node is the target
        if (currentNode.col == target.col && currentNode.row == target.row)
        {
            // We found the shortest path to the food
            state.bug << "We found the shortest path to the food" << endl;
            break;
        }
        else if (nodeEvaluated.size() > MAX_ITERATION)
		{
			// We didn't find the shortest path to the food
			state.bug << "We didn't find the shortest path to the food" << endl;
			break;
		}

        //remove the current node from the set of node to be evaluated
        nodeToEvaluate.erase(nodeToEvaluate.begin());
        //add the current node to the set of node already evaluated
        nodeEvaluated.push_back(currentNode);

        //state.bug << "LocationToEvaluate : " << locationToEvaluate.size() << endl;
        //state.bug << "LocationEvaluated : " << locationEvaluated.size() << endl;

        //Get the possible neighbors of current node
        vector<Node> currentNeighbors = vector<Node>();
        for (int d = 0; d < TDIRECTIONS; d++)
        {
            //float neighborHcost = calculateHeuristic(start, target);
            Node neighbor = Node(&currentNode, 0);
            Location neighborLoc = state.getLocation(currentNode, d);
            neighbor.row = neighborLoc.row;
            neighbor.col = neighborLoc.col;
            neighbor.hCost = calculateHeuristic(neighbor, target);

            // check if the node is not in water or not in the set of node already evaluated
            if (!state.grid[neighbor.row][neighbor.col].isWater && !checkInVector(nodeEvaluated, neighbor))
            {
                currentNeighbors.push_back(neighbor);
            }
        }

        //Set the neighbors to the nodeToEvaluate list
        for (Node neighbor : currentNeighbors) nodeToEvaluate.push_back(neighbor);


        state.bug << "Size Neighbors : " << currentNeighbors.size() << endl;
    }


    state.bug << "The for ended up currectly, now we have to move the ant" << endl;

    
    Node* current = &currentNode;

    // Backtrack from the target node to the start node
    while (current->previousNode != nullptr && current->previousNode->previousNode != nullptr) {
        current = current->previousNode;
    }

    // 'current' is now the node that represents the next move from the start position
    state.bug << "Next move: x=" << current->row << " y=" << current->col << endl;

    Location nextMove = Location(current->row, current->col);


    Location nextLocation = Location(current->previousNode->row, current->previousNode->col);
    return nextLocation;
}


float Astar::calculateHeuristic(const Location a, const Location b) {
    return abs(a.row - b.row) + abs(a.col - b.col); // Manhattan distance because we can only move in 4 directions
    //return sqrt(pow(a.row - b.row, 2) + pow(a.col - b.col, 2)); // Euclidean distance
};



// Create a function which sort like the smallest element appears first
vector<Node> Astar::sort(vector<Node> vec)
{
    Node temp = Node(nullptr, 0);

    bool not_sorted = true;

    for (int i = 0; i < vec.size() && not_sorted; i++) {
        not_sorted = false;
        for (int j = 1; j < vec.size() - i; j++) {

            // Sort by fCost
            if (vec[j - 1].fCost > vec[j].fCost) {
                temp = vec[j - 1];
                vec[j - 1] = vec[j];
                vec[j] = temp;
                not_sorted = true;
            }
            else if (vec[j - 1].fCost == vec[j].fCost)
            {
                // If fCost are equals, sort by hCost
                if (vec[j - 1].hCost > vec[j].hCost) {
                    temp = vec[j - 1];
                    vec[j - 1] = vec[j];
                    vec[j] = temp;
                    not_sorted = true;
                }
            }
        }
    }
    return vec;
};

template <typename T>
bool Astar::checkInVector(const std::vector<T> vec, const T elementToFind) {
    for (T element : vec) {
        //state.bug << "element.row : " << element.row << endl;
        //state.bug << "element.col : " << element.col << endl;

        //state.bug << "elementToFind.row : " << elementToFind.row << endl;
        //state.bug << "elementToFind.col : " << elementToFind.col << endl;

        if (element.col == elementToFind.col && element.row == elementToFind.row) {
            return true; // element found in vector, so return false.
        }
    }
    return false; // element not found in vector, so return true.
}
