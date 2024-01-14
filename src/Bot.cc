#include "Bot.h"
#include "Node.h"
#include "Astar.h"
#include "Timer.h"
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
    //Timer timer;
    //timer.start();

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
        state.bug << "The closest food is : x=" << foodInfoPosition.row << " y=" << foodInfoPosition.col << endl;

        Astar astar = Astar();
        Location nextMove = astar.getNextMove(state.myAnts[i], foodInfoPosition, state);

        // move the ant according the best move found
        for (int d = 0; d < TDIRECTIONS; d++)
        {
            Location loc = state.getLocation(state.myAnts[i], d);
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
    Location itemInfoPosition;

    for (int j = 0; j < items.size(); j++)
    {
        float distance = state.distance(ant, items[j]);
        if (distance < minDistance)
        {
            minDistance = distance;
            //store the position of the closest item
            itemInfoPosition = items[j];

        }
    }
    return itemInfoPosition;
}


//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
