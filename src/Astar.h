/*
authors : Ewen Jezequel, Pierre Vandel
 
/!\ THIS CLASS IS NOT USED FOR THE BOT /!\

Class Astar which is used for the A* algorithm
The A* algorithm was not adpated for the bot, 
so Astar is not called in the bot code

*/


#ifndef ASTAR_H_
#define ASTAR_H_

#include "Node.h"
#include "Location.h"
#include "State.h"


class Astar
{

public:
    Location getNextMove(Location, Location, State&);
        

private:
    float calculateHeuristic(const Location, const Location);

    vector<Node> sort(vector<Node>);

    template <typename T>
    bool checkInVector(const std::vector<T> vec, const T elementToFind);

};

#endif
;