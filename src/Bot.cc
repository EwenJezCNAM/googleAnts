#include "Bot.h"
#include "LocationInfo.h"
#include "NeighborInfo.h"
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

        state.bug << "closest food position : x=" << foodInfoPosition.row << " y=" << foodInfoPosition.col << endl;



        // TODO : Use A* search algorithm for every neighbors of the ant.
        // Then, we move the ant on the neighbor with the shortest path

        Location start = state.myAnts[i];
        Location target = foodInfoPosition;

        //Get the possible neighbors a the start position
        vector<NeighborInfo> neighborInfoList = vector<NeighborInfo>();
            
        vector<Location> startNeighbors = vector<Location>();

        for (int d = 0; d < TDIRECTIONS; d++)
        {
            Location loc = state.getLocation(start, d);
            // check if the location is not in the set of node already evaluated
            if (!state.grid[loc.row][loc.col].isWater)
            {
                startNeighbors.push_back(loc);
            }
        }

        state.bug << "Size startNeighbors :" << startNeighbors.size() << endl;

        for (Location startNeighbor : startNeighbors)
        {
			state.bug << "Start neighbor : x=" << startNeighbor.row << " y=" << startNeighbor.col << endl;        
            vector<LocationInfo> locationToEvaluate = vector<LocationInfo>();//the set of node to be evaluated
            vector<LocationInfo> locationEvaluated = vector<LocationInfo>();//set of node already evaluated

            locationEvaluated.push_back(LocationInfo(start, start, target));

            int pathLenght = 0;


            LocationInfo currentLocation = LocationInfo(startNeighbor, startNeighbor, target); //temporary node for the A*
            locationToEvaluate.push_back(currentLocation);


            //while the set of node to be evaluated is not empty
            while (!locationToEvaluate.empty())
            {
                // Get the next location to test
                locationToEvaluate = sort(locationToEvaluate);
                currentLocation = locationToEvaluate[0];

                if (currentLocation.col == target.col && currentLocation.row == target.row)
			    {
                    neighborInfoList.push_back(NeighborInfo(startNeighbor, pathLenght));
				    // We found the shortest path to the food
                    state.bug <<"We found the shortest path to the food" << endl;
				    break;
			    }

                pathLenght++;


                //remove the current location from the set of node to be evaluated
                locationToEvaluate.erase(locationToEvaluate.begin());
                //add the current location to the set of node already evaluated
                locationEvaluated.push_back(currentLocation);

                //state.bug << "LocationToEvaluate : " << locationToEvaluate.size() << endl;
                //state.bug << "LocationEvaluated : " << locationEvaluated.size() << endl;

                //Get the possible neighbors of current position
                vector<LocationInfo> neighbors = vector<LocationInfo>();
                for (int d = 0; d < TDIRECTIONS; d++)
		        {
			        LocationInfo loc = LocationInfo(state.getLocation(currentLocation, d), start, target);
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

                state.bug << "Size Neighbors : " << neighbors.size() << endl;
            }
        }


        state.bug << "The for ended up currectly, now we have to move the ant" << endl;

        //Get the shortest start neighbor according their path lenght
        state.bug << "size of neighborInfoList : " << neighborInfoList.size() << endl;

        bool not_sorted = true;
        NeighborInfo temp = NeighborInfo();
        for (int n = 1; n < neighborInfoList.size(); n++)
        {
            state.bug << "toujours dans le boucle" << endl;

            // Sort by the calculated distance to the target
            if (neighborInfoList[n - 1].distanceToTarget > neighborInfoList[n].distanceToTarget)
            {
                state.bug << "Passage dans le if" << endl;
                temp = neighborInfoList[n - 1];
                neighborInfoList[n - 1] = neighborInfoList[n];
                neighborInfoList[n] = temp;

                not_sorted = true;
            }
            else
            {
                not_sorted = false;
            }
        }

        state.bug << "The neighborInfoList have been sorted" << endl;

        Location nextMove = neighborInfoList[0].neighborLocation;
        
        state.bug << "The ant will move to : x=" << nextMove.row << " y=" << nextMove.col << endl;

        // move the ant to the first step of the shortest path found
        for (int d = 0; d < TDIRECTIONS; d++)
        {
            Location loc = state.getLocation(nextMove, d);
            if (!state.grid[loc.row][loc.col].isWater)
			{
				state.makeMove(state.myAnts[i], d);
				break;
			}
        }
        
        state.bug << "The ant have been move" << endl;



        

	}

    //state.bug << "turn " << state.turn << ":" << endl;
    //state.bug << state << endl;

    //state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};


bool Bot::checkInVector(const std::vector<LocationInfo> vec, const LocationInfo elementToFind) {
    for (LocationInfo element : vec) {
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


//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
