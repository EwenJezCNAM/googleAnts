#include "Bot.h"
#include "LocationInfo.h"
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
    state.bug << "MyAnts size : " << state.myAnts.size() << endl;
    for (int i = 0; i < state.myAnts.size(); i++)
    {
        state.bug << "MyAnts position : x=" << state.myAnts[i].row << " y=" << state.myAnts[i].col << endl;
    }


    for (int i = 0; i < state.food.size(); i++)
    {
        state.bug << "Food position : x=" << state.food[i].row << " y=" << state.food[i].col << endl;
    }
    
    //NEW CODE
    //Get the closest food to each ants
    for (int i = 0; i < state.myAnts.size(); i++)
	{
		float minDistance = 999999.0f;
        // Create the dict to store the min distance and the position of the closest food
        Location foodInfoPosition;

		for (int j = 0; j < state.food.size(); j++)
		{
			float distance = state.distance(state.myAnts[i], state.food[j]);
			if (distance < minDistance)
			{
				minDistance = distance;
                //store the position of the closest food
                foodInfoPosition = state.food[j];

			}
		}

		// Move the ant to the closest food using no method
        /*
        if (minDistance < 999999.0f) {
			float yDistance = abs(state.myAnts[i].row - foodInfoPosition.row);
            float xDistance = abs(state.myAnts[i].col - foodInfoPosition.col);
        
            if (yDistance > xDistance) {
				if (state.myAnts[i].row > foodInfoPosition.row) {
					state.makeMove(state.myAnts[i], 0);
				}
				else {
					state.makeMove(state.myAnts[i], 2);
				}
			}
			else {
				if (state.myAnts[i].col > foodInfoPosition.col) {
					state.makeMove(state.myAnts[i], 3);
				}
				else {
					state.makeMove(state.myAnts[i], 1);
				}
			}
		}
        */

        // TODO : Use A* search algorithm instead
        vector<LocationInfo> locationToEvaluate; //the set of node to be evaluated
        vector<LocationInfo> locationEvaluated; //set of node already evaluated


        LocationInfo currentLocation = LocationInfo(state.myAnts[i], state.myAnts[i], foodInfoPosition); //temporary node for the A*
        locationToEvaluate.push_back(currentLocation);

        Location start = state.myAnts[i];

        currentLocation.path.push_back(start);

        //while the set of node to be evaluated is not empty
        while (!locationToEvaluate.empty())
        {
            vector<Location> previousPath = currentLocation.path;


            // Get the next location to test
            locationToEvaluate = sort(locationToEvaluate);
            currentLocation = locationToEvaluate[0];
            currentLocation.path = previousPath;

            if (currentLocation.col == foodInfoPosition.col && currentLocation.row == foodInfoPosition.row)
			{
				// We found the shortest path to the food
                state.bug <<"We found the shortest path to the food" << endl;
				break;
			}

            state.bug << "LocationToEvaluate : " << locationToEvaluate.size() << endl;
            state.bug << "LocationEvaluated : " << locationEvaluated.size() << endl;

            //Get the possible neighbors of current position
            vector<LocationInfo> neighbors = vector<LocationInfo>();
            for (int d = 0; d < TDIRECTIONS; d++)
		    {
			    LocationInfo loc = LocationInfo(state.getLocation(currentLocation, d), start, foodInfoPosition);
                // check if the location is not in the set of node already evaluated
			    if (!state.grid[loc.row][loc.col].isWater && !checkInVector(locationEvaluated, loc))
			    {
				    neighbors.push_back(loc);
			    }
		    }

            //Set the neighbors to the location to evaluate list
            for (LocationInfo neighbor : neighbors)
            {
                //neighbor.addPath(currentLocation.path);
			    locationToEvaluate.push_back(neighbor);

		    }

            //remove the current location from the set of node to be evaluated
            locationToEvaluate.erase(locationToEvaluate.begin());
            //add the current location to the set of node already evaluated
            locationEvaluated.push_back(currentLocation);

        }

        state.bug << "The while ended up currectly, now we have to move the ant" << endl;
        
        state.bug << "Current location size : " << currentLocation.path.size() << endl;
        

        //Location nextMove = currentLocation.path[1];
        //state.bug << "Next move : x=" << currentLocation.path[1].row << " y=" << currentLocation.path[1].col << endl;
        //
        //
        //state.bug << "Get the next move" << endl;
        //// move the ant to the first step of the shortest path found
        //for (int d = 0; d < TDIRECTIONS; d++)
        //{
        //    Location loc = state.getLocation(currentLocation, d);
        //    if (loc == nextMove)
		//	{
		//		state.makeMove(state.myAnts[i], d);
		//		break;
		//	}
        //}
        //
        //state.bug << "The ant have been move" << endl;



        

	}

    // ORIGINAL CODE
    //state.bug << "turn " << state.turn << ":" << endl;
    //state.bug << state << endl;
    
    
    //picks out moves for each ant
    //for(int ant=0; ant<(int)state.myAnts.size(); ant++)
    //{
    //    for(int d=0; d<TDIRECTIONS; d++)
    //    {
    //        Location loc = state.getLocation(state.myAnts[ant], d);
    //
    //        if(!state.grid[loc.row][loc.col].isWater)
    //        {
    //            state.makeMove(state.myAnts[ant], d);
    //            break;
    //        }
    //    }
    //}

    //state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};


bool Bot::checkInVector(const std::vector<LocationInfo> vec, const LocationInfo elementToFind) {
    for (LocationInfo element : vec) {
        if (element.col == elementToFind.col && element.row == elementToFind.row) {
            return true; // element found in vector, so return false.
        }
    }
    return false; // element not found in vector, so return true.
}

int Bot::calculateFCost(int gCost, Location n, Location goal) {
    //f(n) = g(n) + h(n)
    //int gCost = 0; // g(n) is the cost of the path from the start node to n
    int hCost = calculateHeuristic(n, goal); // h(n) is the heuristique fonction which estimates the cost of the cheapest path from n to the goal

    int fCost = gCost + hCost;
    return fCost;
}

// Create a function which sort like the smallest element appears first
vector<LocationInfo> Bot::sort(vector<LocationInfo> vec)
{
    LocationInfo temp = LocationInfo();

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

float Bot::calculateHeuristic(const Location a, const Location b) {
    return abs(a.row - b.row) + abs(a.col - b.col); // Manhattan distance because we can only move in 4 directions
}


//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
