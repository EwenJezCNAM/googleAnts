/*
authors : Ewen Jezequel, Pierre Vandel
*/

#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include "Location.h"
#include "AntLogic.h"
#include <vector>

/*
    This struct represents your bot in the game of Ants
*/
struct Bot
{
    State m_state;

    vector<AntLogic> m_knownAnts;
    vector<Location> m_nextsAntsLocation;
    vector<AntLogic> m_notMovedAnts;


    Bot();

    void playGame();    //plays a single game of Ants

    void makeMoves();   //makes moves for a single turn

    Location getClosestItem(Location ant, vector<Location> items);

    // std::vector<LocationInfo> sort(std::vector<LocationInfo>);

    template <typename T>
    bool checkInVector(const std::vector<T>, const T);

    void endTurn();     //indicates to the engine that it has made its moves
    
};

#endif //BOT_H_
