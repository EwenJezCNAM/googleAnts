/*
authors : Ewen Jezequel, Pierre Vandel
*/

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
    cin >> m_state;
    m_state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> m_state)
    {
        m_state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    m_nextsAntsLocation.clear(); // Clearing all the previous future ants location because it is now their location

    for (int i = 0; i < m_knownAnts.size(); i++) // Clearing my list of all the dead ants
    {
        bool dead = true;
        for (int j = 0; j < m_state.myAnts.size(); j++) {
            if (m_knownAnts[i].m_antLocation == m_state.myAnts[j]) {
                dead = false;
            }
        }
        if (dead) {
            m_knownAnts.erase(m_knownAnts.begin() + i);
        }
    }

    for (int i = 0; i < m_state.myAnts.size(); i++) // Adding new ants to my list of known ant so i can make them move and give them an objective
    {
        bool inTheList = false;
        for (int j = 0; j < m_knownAnts.size(); j++) {
            if (m_knownAnts[j].m_antLocation == m_state.myAnts[i]) {
                inTheList = true;
            }
        }
        if (!inTheList) {
            m_knownAnts.insert(m_knownAnts.begin(), AntLogic(m_state.myAnts[i], getClosestItem(m_state.myAnts[i], m_state.food)));
        }
    }

    m_notMovedAnts = m_knownAnts; // Updating not moved ants so thay don't kill themselfs

    for (int i = 0; i < m_knownAnts.size(); i++) { // Giving all the ants their next move
        Location closestFood = getClosestItem(m_knownAnts[i].m_antLocation, m_state.food); // Location the nearest food of the current ant
        int nextDirection = m_knownAnts[i].getNextMove(closestFood, m_state, m_nextsAntsLocation, m_notMovedAnts); // get the next move of the current ant
        if (nextDirection != 4) { // if not 4 applying the next move and refresh it's position in my list
            m_state.makeMove(m_knownAnts[i].m_antLocation, nextDirection);
            m_knownAnts[i].refreshPosition(nextDirection, m_state);
            m_nextsAntsLocation.insert(m_nextsAntsLocation.begin(), m_knownAnts[i].m_antLocation);
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
        float distance = m_state.distance(ant, items[j]);
        if (distance < minDistance)
        {
            minDistance = distance;
            //store the position of the closest item
            foodInfoPosition = items[j];

        }
    }
    return foodInfoPosition;
}

bool isInList(vector<AntLogic> list, Location Ant) {
    for (int i = 0; i < list.size(); i++) {
        if (list[i].m_antLocation == Ant)
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
    if(m_state.turn > 0)
        m_state.reset();
    m_state.turn++;

    cout << "go" << endl;
};
