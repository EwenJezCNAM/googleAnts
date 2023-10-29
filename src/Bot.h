#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include "Location.h"
#include "LocationInfo.h"
#include "State.h"
#include <vector>

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State state;

    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn

    int calculateFCost(int, Location, Location); // calculate the f cost of the current state

    std::vector<LocationInfo> sort(std::vector<LocationInfo>);

    bool checkInVector(const std::vector<LocationInfo>, const LocationInfo);

    float calculateHeuristic(const Location a, const Location b); // calculate the Manhattan distance

    void endTurn();     //indicates to the engine that it has made its moves
    
};

#endif //BOT_H_
