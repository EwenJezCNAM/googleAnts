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
    NextsAntsLocation.clear();
    //Set the next move for each ant
    for (int i = 0; i < state.myAnts.size(); i++)
    {
        boolean inTheList = false;
        for (int j = 0; j < KnownAnts.size(); j++) {
            if (KnownAnts[j].AntLocation == state.myAnts[i]) {
                inTheList = true;
            }
        }
        if (!inTheList) {
            KnownAnts.insert(KnownAnts.begin(), AntLogic(state.myAnts[i], getClosestItem(state.myAnts[i], state.food), false));
        }
    }

    for (int i = 0; i < KnownAnts.size(); i++)
    {
        boolean dead = true;
        for (int j = 0; j < state.myAnts.size(); j++) {
            if (KnownAnts[i].AntLocation == state.myAnts[j]) {
                dead = false;
            }
        }
        if (dead) {
            KnownAnts.erase(KnownAnts.begin() + i);
        }
    }

    notMovedAnts = KnownAnts;

    for (int i = 0; i < KnownAnts.size(); i++) {
        Location closestFood = getClosestItem(KnownAnts[i].AntLocation, state.food);
        int nextDirection = KnownAnts[i].GetNextMove(closestFood, state, NextsAntsLocation, notMovedAnts);
        if (nextDirection != 4) {
            state.makeMove(KnownAnts[i].AntLocation, nextDirection);
            KnownAnts[i].refreshPosition(nextDirection, state);
            notMovedAnts.erase(notMovedAnts.begin());
            NextsAntsLocation.insert(NextsAntsLocation.begin(), KnownAnts[i].AntLocation);
        }
    }
 //       // Get the closest food to the ant
	//	Location foodInfoPosition = getClosestItem(state.myAnts[i], state.food);
 //       state.bug <<"The closest food is : x=" << foodInfoPosition.row << " y=" << foodInfoPosition.col << endl;

 //       Location NextCurrentAntMove = Astar(state.myAnts[0],foodInfoPosition);

 //       // Use A* search algorithm for every neighbors of the ant.
 //       // Then, we move the ant on the neighbor according the shortest path
 //       
 //       state.bug << "The ant will move to : x=" << begining->getNextLocation()->row << " y=" << begining->getNextLocation()->col << endl;

 //       Location nextMove = Location(begining->getNextLocation()->row, begining->getNextLocation()->col);




 //       // move the ant according the best move found
 //       for (int d = 0; d < TDIRECTIONS; d++)
 //       {
 //           Location loc = state.getLocation(start, d);
 //           if (loc.row == nextMove.row && loc.col == nextMove.col)
 //           {
	//		    state.makeMove(state.myAnts[i], d);
	//		    break;
 //           }
 //       }

 //       state.bug << "The ant have been move" << endl;
	//}
 //   state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;

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

boolean isInList(vector<AntLogic> list, Location Ant) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].AntLocation == Ant)
            return true;
    }
    return false;
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



//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};
