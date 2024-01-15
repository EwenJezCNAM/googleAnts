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
    NextsAntsLocation.clear(); // Clearing all the previous future ants location because it is now their location

    for (int i = 0; i < KnownAnts.size(); i++) // Clearing my list of all the dead ants
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

    for (int i = 0; i < state.myAnts.size(); i++) // Adding new ants to my list of known ant so i can make them move and give them an objective
    {
        boolean inTheList = false;
        for (int j = 0; j < KnownAnts.size(); j++) {
            if (KnownAnts[j].AntLocation == state.myAnts[i]) {
                inTheList = true;
            }
        }
        if (!inTheList) {
            KnownAnts.insert(KnownAnts.begin(), AntLogic(state.myAnts[i], getClosestItem(state.myAnts[i], state.food)));
        }
    }

    notMovedAnts = KnownAnts; // Updating not moved ants so thay don't kill themselfs

    for (int i = 0; i < KnownAnts.size(); i++) { // Giving all the ants theire next move
        Location closestFood = getClosestItem(KnownAnts[i].AntLocation, state.food); // Location the nearest food of the current ant
        int nextDirection = KnownAnts[i].GetNextMove(closestFood, state, NextsAntsLocation, notMovedAnts); // get the next move of the current ant
        if (nextDirection != 4) { // if not 4 applying the next move and refresh it's position in my list
            state.makeMove(KnownAnts[i].AntLocation, nextDirection);
            KnownAnts[i].refreshPosition(nextDirection, state);
            NextsAntsLocation.insert(NextsAntsLocation.begin(), KnownAnts[i].AntLocation);
        }
    }
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
