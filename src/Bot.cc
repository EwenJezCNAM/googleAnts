#include "Bot.h"
#include "Node.h"
#include <map>
#include <iostream>
using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
    cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    //state.bug << state << endl;

    state.bug << "MyAnts size : " << state.myAnts.size() << endl;
    for (int i = 0; i < state.myAnts.size(); i++)
    {
        state.bug << "MyAnts position : x=" << state.myAnts[i].row << " y=" << state.myAnts[i].col << endl;
    }


    for (int i = 0; i < state.food.size(); i++)
    {
        state.bug << "Food position : x=" << state.food[i].row << " y=" << state.food[i].col << endl;
    }
    

    //Set the next move for each ant
    for (int i = 0; i < state.myAnts.size(); i++)
	{
        // Get the closest food to the ant
		Location foodInfoPosition = getClosestItem(state.myAnts[i], state.food);
        state.bug <<"The closest food is : x=" << foodInfoPosition.row << " y=" << foodInfoPosition.col << endl;

        // Use A* search algorithm for every neighbors of the ant.
        // Then, we move the ant on the neighbor according the shortest path found

        Location start = state.myAnts[i];
        Location target = foodInfoPosition;

        vector<Node> nodeToEvaluate = vector<Node>();//the set of node to be evaluated
        vector<Node> nodeEvaluated = vector<Node>();//set of node already evaluated

        Node currentNode = Node(start, start, target);

        nodeToEvaluate.push_back(currentNode);

        Node previousNode;
        
        //while the set of node to be evaluated is not empty
        while (!nodeToEvaluate.empty())
        {
            // Get the next node to test (the node with the lowest f cost)
            nodeToEvaluate = sort(nodeToEvaluate);
            currentNode = nodeToEvaluate[0];

            state.bug << "test" << endl;
            currentNode.setPreviousNode(previousNode);
            previousNode = currentNode;


            state.bug << "currentLocation, x=" << currentNode.row << "y=" << currentNode.col << endl;

            // Check if the node is the target
            if (currentNode.col == target.col && currentNode.row == target.row)
			{
                // We found the shortest path to the food
                state.bug <<"We found the shortest path to the food" << endl;
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
                Node neighbor = Node(state.getLocation(currentNode, d), start, target);
                // check if the node is not in the set of node already evaluated
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


        bool beginingFound = false;
        Node* begining = currentNode.getPreviousNode();

        while (!beginingFound)
        {
            begining = begining->getPreviousNode();
            state.bug << "begining : " << begining->getPreviousNode()->row << " " << begining->getPreviousNode()->col << endl;

            if (!begining)
            {
                beginingFound = true;
                break;
            }
        }
        
        state.bug << "The ant will move to : x=" << begining->getNextNode()->row << " y=" << begining->getNextNode()->col << endl;

        Location nextMove = Location(begining->getNextNode()->row, begining->getNextNode()->col);

        // move the ant according the best move found
        for (int d = 0; d < TDIRECTIONS; d++)
        {
            Location loc = state.getLocation(start, d);
            if (loc.row == nextMove.row && loc.col == nextMove.col)
            {
			    state.makeMove(state.myAnts[i], d);
			    break;
            }

        }
        
        state.bug << "The ant have been move" << endl;
	}


    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};


Location Bot::getClosestItem(Location ant, vector<Location> items)
{
    float minDistance = 999999.0f;
    // Create the dict to store the min distance and the position of the closest item
    Location foodInfoPosition;

    for (int j = 0; j < items.size(); j++)
    {
        float distance = state.distance(ant, items[j]);
        if (distance < minDistance)
        {
            minDistance = distance;
            //store the position of the closest item
            foodInfoPosition = items[j];

        }
    }
    return foodInfoPosition;
}

template <typename T>
bool Bot::checkInVector(const std::vector<T> vec, const T elementToFind) {
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


// Create a function which sort like the smallest element appears first
vector<Node> Bot::sort(vector<Node> vec)
{
    Node temp = Node();

    bool not_sorted = true;

    for (int i = 0; i < vec.size() && not_sorted; i++) {
        not_sorted = false;
        for (int j = 1; j < vec.size() - i; j++) {
            
            // Sort by fCost
            if (vec[j - 1].fCost > vec[j].fCost) {
                temp = vec[j - 1];
                vec[j-1] = vec[j];
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


//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
