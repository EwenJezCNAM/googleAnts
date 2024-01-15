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
    Location AntLocation; // Storing the Antlocation so I know wich one is it
private:
    Location Objectif; // Storing the objective of the ant so it can remember and keep going in the same direction
    int lastMove; // Remembering the last move done so the ant don't get stuck in a loop
    // The four list are used so an ant don't try to go in the same direction continuously
    // For exemple the ant try a dead end, it'll go bakc on it's step and remember that this column
    // Don't help it in it's way to the food. It will then try an other way around
    vector<int> bannedLeftRow;
    vector<int> bannedRightRow;
    vector<int> bannedUpCol;
    vector<int> bannedDownCol;


public:
    AntLogic(Location _Antloc, Location _objectivLoc) 
    {
        // Creation of the ant and attribution its objective
        AntLocation = _Antloc;
        Objectif = _objectivLoc;
    };

    int GetNextMove(Location newObjectivLoc, State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // Fuction returning the next move of a selected ant as an int
        if (Objectif != newObjectivLoc) {
            // If the objective change, the ant will forget the dead end of the last objective
            clearAllList();
            Objectif = newObjectivLoc;
        }
        checkAndAddPositionToBanList(state);
        int rowDirection = AntLocation.row - Objectif.row;
        int colDirection = AntLocation.col - Objectif.col;
        // Here i change the result of rowDirection and colDirection depending on their result so
        // my ant don't go across the map to grab food that are on the other side of it but on the edge of it
        if (rowDirection > 80) {
            rowDirection = -1;
        }
        if (colDirection > 50) {
            colDirection = -1;
        }
        if (rowDirection < -80) {
            rowDirection = 1;
        }
        if (colDirection < -50) {
            colDirection = 1;
        }
        int decision = decisionMaking(rowDirection, colDirection, state, NextsAntsLocation, currentAntsLocation);
        return decision;
    };

    boolean checkIfBanned(vector<int> testedList, int testedInt) {
        // This function check if an int is or not in a list of int

        boolean isBanned = false;
        for (int i = 0; i < testedList.size(); i++) {
            if (testedInt == testedList[i]) {
                isBanned = true;
                break;
            }
        }
        return isBanned;
    };

    boolean checkInList(vector<Location> NextMoveList, vector<AntLogic> currentAntsLocation, Location testedInt) {
        // This function check if a location is or not in on of the 2 list so the ants 
        // don't kill them self by going on the same location
        
        boolean isInList = false;
        for (int i = 0; i < NextMoveList.size(); i++) {
            if (testedInt == NextMoveList[i]) {
                isInList = true;
                break;
            }
        }
        if (!isInList) {
            for (int i = 0; i < currentAntsLocation.size(); i++) {
                if (testedInt == currentAntsLocation[i].AntLocation) {
                    isInList = true;
                    break;
                }
            }
        }
        return isInList;
    };


    void checkAndAddPositionToBanList(State& state) {
        // Function Checking if the column or row is water and is not already in this banned list and if so add it to the ban list
        Location northneighbor = state.getLocation(AntLocation, 0);
        Location southneighbor = state.getLocation(AntLocation, 2);
        Location westneighbor = state.getLocation(AntLocation, 3);
        Location easthneighbor = state.getLocation(AntLocation, 1);

        if (state.grid[northneighbor.row][northneighbor.col].isWater && !checkIfBanned(bannedUpCol, AntLocation.col)) {
            bannedUpCol.insert(bannedUpCol.begin(), AntLocation.col);
        }
        if (state.grid[southneighbor.row][southneighbor.col].isWater && !checkIfBanned(bannedDownCol, AntLocation.col)) {
            bannedDownCol.insert(bannedDownCol.begin(), AntLocation.col);
        }
        if (state.grid[westneighbor.row][westneighbor.col].isWater && !checkIfBanned(bannedLeftRow, AntLocation.row)) {
            bannedLeftRow.insert(bannedLeftRow.begin(), AntLocation.row);
        }
        if (state.grid[easthneighbor.row][easthneighbor.col].isWater && !checkIfBanned(bannedRightRow, AntLocation.row)) {
            bannedRightRow.insert(bannedRightRow.begin(), AntLocation.row);
        }
    };

    void clearAllList() {
        // Function clearing all the "memory" of the ant so it don't get stuck if a new objectif is determined
        bannedUpCol.clear();
        bannedLeftRow.clear();
        bannedDownCol.clear();
        bannedRightRow.clear();
    };

    int decisionMaking(int rowDirection, int colDirection, State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // Depending of the result of d1 and d2, I return a specific function so the nat can go to the food the fastest way possible
        if (rowDirection >= 0 && colDirection <= 0) {
            // The objective is in the north-east direction depending on the ant
            return moveNorthEast(state, NextsAntsLocation, currentAntsLocation);
        }
        if (rowDirection <= 0 && colDirection <= 0) {
            // The objective is in the south-east direction depending on the ant
            return moveSouthEast(state, NextsAntsLocation, currentAntsLocation);
        }
        if (rowDirection <= 0 && colDirection >= 0) {
            // The objective is in the south-west direction depending on the ant
            return moveSouthWest(state, NextsAntsLocation, currentAntsLocation);
        }
        if (rowDirection >= 0 && colDirection >= 0) {
            // The objective is in the north-west direction depending on the ant
            return moveNorthWest(state, NextsAntsLocation, currentAntsLocation);
        }
    };

    int moveNorthEast(State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // In case the food is in the north-east of the ant, it will try to go up first, then, if it can't, it'll try to go right, then down and then left
        // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
        // And if it can't move at all it will return 4 making it stay put
        if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 0))) {
            bannedUpCol.clear();
            lastMove = 0;
            return 0;
        }
        else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 1))) {
            bannedRightRow.clear();
            lastMove = 1;
            return 1;
        }
        else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 2))) {
            bannedDownCol.clear();
            lastMove = 2;
            return 2;
        }
        else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 3))) {
            bannedLeftRow.clear();
            lastMove = 3;
            return 3;
        }
        else if (!checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 0))) {
            lastMove = 2;
            return 2;
        }
        else {
            return 4;
        }
    }

    int moveNorthWest(State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // In case the food is in the north-west of the ant, it will try to go left first, then, if it can't, it'll try up, then right and then down
        // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
        // And if it can't move at all it will return 4 making it stay put
        if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 3))) {
            bannedLeftRow.clear();
            lastMove = 3;
            return 3;
        }
        else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 0))) {
            bannedUpCol.clear();
            lastMove = 0;
            return 0;
        }
        else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 1))) {
            bannedRightRow.clear();
            lastMove = 1;
            return 1;
        }
        else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 2))) {
            bannedDownCol.clear();
            lastMove = 2;
            return 2;
        }
        else if (!checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 3))) {
            lastMove = 3;
            return 3;
        }
        else {
            return 4;
        }
    }

    int moveSouthEast(State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // In case the food is in the south-east of the ant, it will try to go right first, then, if it can't, it'll go down, then left and then up
        // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
        // And if it can't move at all it will return 4 making it stay put
        if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 1))) {
            bannedRightRow.clear();
            lastMove = 1;
            return 1;
        }
        else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 2))) {
            bannedDownCol.clear();
            lastMove = 2;
            return 2;
        }
        else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 3))) {
            bannedLeftRow.clear();
            lastMove = 3;
            return 3;
        }
        else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 0))) {
            bannedUpCol.clear();
            lastMove = 0;
            return 0;
        }
        else if (!checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 1))) {
            lastMove = 1;
            return 1;
        }
        else {
            return 4;
        }
    }

    int moveSouthWest(State& state, vector<Location> NextsAntsLocation, vector<AntLogic> currentAntsLocation) {
        // In case the food is in the south-west of the ant, it will try to go down first, then, if it can't, it'll try left, then up and then right
        // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
        // And if it can't move at all it will return 4 making it stay put
        if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 2))) {
            bannedDownCol.clear();
            lastMove = 2;
            return 2;
        }
        else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 3))) {
            bannedLeftRow.clear();
            lastMove = 3;
            return 3;
        }
        else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 0))) {
            bannedUpCol.clear();
            lastMove = 0;
            return 0;
        }
        else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 1))) {
            bannedRightRow.clear();
            lastMove = 1;
            return 1;
        }
        else if (!checkInList(NextsAntsLocation, currentAntsLocation, state.getLocation(AntLocation, 2))) {
            lastMove = 2;
            return 2;
        }
        else {
            return 4;
        }
    }

    void refreshPosition(int nextDirection, State& state) {
        // I update the position of the ant depending on the move inserted
        AntLocation = state.getLocation(AntLocation, nextDirection);
    }
};