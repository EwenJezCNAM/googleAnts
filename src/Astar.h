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