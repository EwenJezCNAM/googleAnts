#ifndef ANTLOGIC_H_
#define ANTLOGIC_H_

#include "State.h"
#include "Location.h"
#include "Square.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
using std::vector;

struct AntLogic
{
    Location m_antLocation; // Storing the antlocation so I know wich one is it

private:
    Location m_objectif; // Storing the objective of the ant so it can remember and keep going in the same direction
    int m_lastMove; // Remembering the last move done so the ant don't get stuck in a loop
    // The four list are used so an ant don't try to go in the same direction continuously
    // For exemple the ant try a dead end, it'll go back on it's step and remember that this column
    // Don't help it in it's way to the food. It will then try an other way around

    vector<int> m_bannedLeftRow;
    vector<int> m_bannedRightRow;
    vector<int> m_bannedUpCol;
    vector<int> m_bannedDownCol;


public:
    AntLogic(Location, Location);

    int getNextMove(Location, State&, vector<Location>, vector<AntLogic>);

    bool checkIfBanned(vector<int>, int);

    bool checkInList(vector<Location>, vector<AntLogic>, Location);

    void checkAndAddPositionToBanList(State&);

    void clearAllList();

    int decisionMaking(int, int, State&, vector<Location>, vector<AntLogic>);

    int moveNorthEast(State&, vector<Location>, vector<AntLogic>);
    int moveNorthWest(State&, vector<Location>, vector<AntLogic>);
    int moveSouthEast(State&, vector<Location>, vector<AntLogic>);
    int moveSouthWest(State&, vector<Location>, vector<AntLogic>);

    void refreshPosition(int, State&);
};

#endif //ANTLOGIC_H_