#include "Bot.h"

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

		// Move the ant to the closest food using no method
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
        // TODO : Use A* search algorithm instead
        






	}



    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;
    
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

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
