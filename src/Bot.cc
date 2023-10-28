#include "Bot.h"
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
        //vector<Location> frontier;
        //frontier.push_back(state.myAnts[i]);
        //map<Location, Location> came_from;
        //map<Location, float> cost_so_far;
        //came_from[state.myAnts[i]] = state.myAnts[i];
        //cost_so_far[state.myAnts[i]] = 0;
        //
        //while (!frontier.empty()) {
		//	Location current = frontier.front();
		//	frontier.erase(frontier.begin());
        //
		//	if (current.row == foodInfoPosition.row && current.col == foodInfoPosition.col) {
		//		break;
		//	}
        //
		//	for (int d = 0; d < TDIRECTIONS; d++) {
		//		Location next = state.getLocation(current, d);
		//		if (state.grid[next.row][next.col].isWater) {
		//			continue;
		//		}
		//		float new_cost = cost_so_far[current] + 1;
		//		if (cost_so_far.find(next) == cost_so_far.end() || new_cost < cost_so_far[next]) {
		//			cost_so_far[next] = new_cost;
		//			frontier.push_back(next);
		//			came_from[next] = current;
		//		}
		//	}
		//}
        //// Reconstruct the path
		//Location current = foodInfoPosition;
		//while (current.row != state.myAnts[i].row || current.col != state.myAnts[i].col) {
		//	Location next = came_from[current];
		//	if (next.row == current.row) {
		//		if (next.col > current.col) {
		//			state.makeMove(state.myAnts[i], 1);
		//		}
		//		else {
		//			state.makeMove(state.myAnts[i], 3);
		//		}
		//	}
		//	else {
		//		if (next.row > current.row) {
		//			state.makeMove(state.myAnts[i], 2);
		//		}
		//		else {
		//			state.makeMove(state.myAnts[i], 0);
		//		}
		//	}
		//	current = next;
		//}

        vector<Location> tile_to_evaluate; //the set of node to be evaluated
        vector<Location> tile_evaluated; //set of node already evaluated

        tile_to_evaluate.push_back(state.myAnts[i]);
        for (int j = 0; j < tile_to_evaluate.size(); j++)
        {
            int gCost = 0;
            //Location current = sortByDistance(state.myAnts[i], tile_to_evaluate); //Get the smallest

        }
        

	}


    //state.bug << "turn " << state.turn << ":" << endl;
    //state.bug << state << endl;
    
    // ORIGINAL CODE
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


int Bot::calculateFCost(int gCost, Location n, Location goal) {
    //f(n) = g(n) + h(n)
    //int gCost = 0; // g(n) is the cost of the path from the start node to n
    int hCost = calculateHeuristic(n, goal); // h(n) is the heuristique fonction which estimates the cost of the cheapest path from n to the goal

    int fCost = gCost + hCost;
    return fCost;
}

// Create a function which sort like the smallest element appears first
vector<Location> Bot::sortByDistance(Location currentLocation, vector<Location> vec)
{
    Location temp;

    bool not_sorted = true;

    for (int i = 0; i < vec.size() && not_sorted; i++) {
        not_sorted = false;
        for (int j = 1; j < vec.size() - i; j++) {
            
            // Compare the Manhattan dirstance between two elements from the list
            if (calculateHeuristic(currentLocation, vec[j - 1]) > calculateHeuristic(currentLocation, vec[j])) {
                temp = vec[j - 1];
                vec[j-1] = vec[j];
                vec[j] = temp;
                not_sorted = true;
            }
        }
    }
    return vec;
};

float Bot::calculateHeuristic(const Location a, const Location b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}


//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
