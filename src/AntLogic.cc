#include "AntLogic.h"
#include "State.h"
#include "Location.h"
#include "Square.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
using std::vector;


AntLogic::AntLogic(Location _antloc, Location _objectivLoc)
{
    // Creation of the ant and attribution its objective
    m_antLocation = _antloc;
    m_objectif = _objectivLoc;
};

int AntLogic::getNextMove(Location newObjectivLoc, State& state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // Fuction returning the next move of a selected ant as an int
    if (m_objectif != newObjectivLoc) {
        // If the objective change, the ant will forget the dead end of the last objective
        clearAllList();
        m_objectif = newObjectivLoc;
    }
    checkAndAddPositionToBanList(state);
    int rowDirection = m_antLocation.row - m_objectif.row;
    int colDirection = m_antLocation.col - m_objectif.col;
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
    int decision = decisionMaking(rowDirection, colDirection, state, nextsAntsLocation, currentAntsLocation);
    return decision;
};

bool AntLogic::checkIfBanned(vector<int> testedList, int testedInt) {
    // This function check if an int is or not in a list of int

    bool isBanned = false;
    for (int i = 0; i < testedList.size(); i++) {
        if (testedInt == testedList[i]) {
            isBanned = true;
            break;
        }
    }
    return isBanned;
};

bool AntLogic::checkInList(vector<Location> nextMoveList, vector<AntLogic> currentAntsLocation, Location testedInt) {
    // This function check if a location is or not in on of the 2 list so the ants 
    // don't kill them self by going on the same location

    bool isInList = false;
    for (int i = 0; i < nextMoveList.size(); i++) {
        if (testedInt == nextMoveList[i]) {
            isInList = true;
            break;
        }
    }
    if (!isInList) {
        for (int i = 0; i < currentAntsLocation.size(); i++) {
            if (testedInt == currentAntsLocation[i].m_antLocation) {
                isInList = true;
                break;
            }
        }
    }
    return isInList;
};


void AntLogic::checkAndAddPositionToBanList(State& r_state) {
    // Function Checking if the column or row is water and is not already in this banned list and if so add it to the ban list
    Location northneighbor = r_state.getLocation(m_antLocation, 0);
    Location southneighbor = r_state.getLocation(m_antLocation, 2);
    Location westneighbor = r_state.getLocation(m_antLocation, 3);
    Location easthneighbor = r_state.getLocation(m_antLocation, 1);

    if (r_state.grid[northneighbor.row][northneighbor.col].isWater && !checkIfBanned(m_bannedUpCol, m_antLocation.col)) {
        m_bannedUpCol.insert(m_bannedUpCol.begin(), m_antLocation.col);
    }
    if (r_state.grid[southneighbor.row][southneighbor.col].isWater && !checkIfBanned(m_bannedDownCol, m_antLocation.col)) {
        m_bannedDownCol.insert(m_bannedDownCol.begin(), m_antLocation.col);
    }
    if (r_state.grid[westneighbor.row][westneighbor.col].isWater && !checkIfBanned(m_bannedLeftRow, m_antLocation.row)) {
        m_bannedLeftRow.insert(m_bannedLeftRow.begin(), m_antLocation.row);
    }
    if (r_state.grid[easthneighbor.row][easthneighbor.col].isWater && !checkIfBanned(m_bannedRightRow, m_antLocation.row)) {
        m_bannedRightRow.insert(m_bannedRightRow.begin(), m_antLocation.row);
    }
};

void AntLogic::clearAllList() {
    // Function clearing all the "memory" of the ant so it don't get stuck if a new objectif is determined
    m_bannedUpCol.clear();
    m_bannedLeftRow.clear();
    m_bannedDownCol.clear();
    m_bannedRightRow.clear();
};

int AntLogic::decisionMaking(int rowDirection, int colDirection, State& r_state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // Depending of the result of d1 and d2, I return a specific function so the ant can go to the food the fastest way possible
    if (rowDirection >= 0 && colDirection <= 0) {
        // The objective is in the north-east direction depending on the ant
        return moveNorthEast(r_state, nextsAntsLocation, currentAntsLocation);
    }
    if (rowDirection <= 0 && colDirection <= 0) {
        // The objective is in the south-east direction depending on the ant
        return moveSouthEast(r_state, nextsAntsLocation, currentAntsLocation);
    }
    if (rowDirection <= 0 && colDirection >= 0) {
        // The objective is in the south-west direction depending on the ant
        return moveSouthWest(r_state, nextsAntsLocation, currentAntsLocation);
    }
    if (rowDirection >= 0 && colDirection >= 0) {
        // The objective is in the north-west direction depending on the ant
        return moveNorthWest(r_state, nextsAntsLocation, currentAntsLocation);
    }
};

int AntLogic::moveNorthEast(State& r_state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // In case the food is in the north-east of the ant, it will try to go up first, then, if it can't, it'll try to go right, then down and then left
    // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
    // And if it can't move at all it will return 4 making it stay put
    if (!checkIfBanned(m_bannedUpCol, m_antLocation.col) && m_lastMove != 2 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 0))) {
        m_bannedUpCol.clear();
        m_lastMove = 0;
        return 0;
    }
    else if (!checkIfBanned(m_bannedRightRow, m_antLocation.row) && m_lastMove != 3 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 1))) {
        m_bannedRightRow.clear();
        m_lastMove = 1;
        return 1;
    }
    else if (!checkIfBanned(m_bannedDownCol, m_antLocation.col) && m_lastMove != 0 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 2))) {
        m_bannedDownCol.clear();
        m_lastMove = 2;
        return 2;
    }
    else if (!checkIfBanned(m_bannedLeftRow, m_antLocation.row) && m_lastMove != 1 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 3))) {
        m_bannedLeftRow.clear();
        m_lastMove = 3;
        return 3;
    }
    else if (!checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 0))) {
        m_lastMove = 2;
        return 2;
    }
    else {
        return 4;
    }
}

int AntLogic::moveNorthWest(State& r_state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // In case the food is in the north-west of the ant, it will try to go left first, then, if it can't, it'll try up, then right and then down
    // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
    // And if it can't move at all it will return 4 making it stay put
    if (!checkIfBanned(m_bannedLeftRow, m_antLocation.row) && m_lastMove != 1 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 3))) {
        m_bannedLeftRow.clear();
        m_lastMove = 3;
        return 3;
    }
    else if (!checkIfBanned(m_bannedUpCol, m_antLocation.col) && m_lastMove != 2 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 0))) {
        m_bannedUpCol.clear();
        m_lastMove = 0;
        return 0;
    }
    else if (!checkIfBanned(m_bannedRightRow, m_antLocation.row) && m_lastMove != 3 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 1))) {
        m_bannedRightRow.clear();
        m_lastMove = 1;
        return 1;
    }
    else if (!checkIfBanned(m_bannedDownCol, m_antLocation.col) && m_lastMove != 0 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 2))) {
        m_bannedDownCol.clear();
        m_lastMove = 2;
        return 2;
    }
    else if (!checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 3))) {
        m_lastMove = 3;
        return 3;
    }
    else {
        return 4;
    }
}

int AntLogic::moveSouthEast(State& state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // In case the food is in the south-east of the ant, it will try to go right first, then, if it can't, it'll go down, then left and then up
    // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
    // And if it can't move at all it will return 4 making it stay put
    if (!checkIfBanned(m_bannedRightRow, m_antLocation.row) && m_lastMove != 3 && !checkInList(nextsAntsLocation, currentAntsLocation, state.getLocation(m_antLocation, 1))) {
        m_bannedRightRow.clear();
        m_lastMove = 1;
        return 1;
    }
    else if (!checkIfBanned(m_bannedDownCol, m_antLocation.col) && m_lastMove != 0 && !checkInList(nextsAntsLocation, currentAntsLocation, state.getLocation(m_antLocation, 2))) {
        m_bannedDownCol.clear();
        m_lastMove = 2;
        return 2;
    }
    else if (!checkIfBanned(m_bannedLeftRow, m_antLocation.row) && m_lastMove != 1 && !checkInList(nextsAntsLocation, currentAntsLocation, state.getLocation(m_antLocation, 3))) {
        m_bannedLeftRow.clear();
        m_lastMove = 3;
        return 3;
    }
    else if (!checkIfBanned(m_bannedUpCol, m_antLocation.col) && m_lastMove != 2 && !checkInList(nextsAntsLocation, currentAntsLocation, state.getLocation(m_antLocation, 0))) {
        m_bannedUpCol.clear();
        m_lastMove = 0;
        return 0;
    }
    else if (!checkInList(nextsAntsLocation, currentAntsLocation, state.getLocation(m_antLocation, 1))) {
        m_lastMove = 1;
        return 1;
    }
    else {
        return 4;
    }
}

int AntLogic::moveSouthWest(State& r_state, vector<Location> nextsAntsLocation, vector<AntLogic> currentAntsLocation) {
    // In case the food is in the south-west of the ant, it will try to go down first, then, if it can't, it'll try left, then up and then right
    // If the ant is stuck and no one is behind it, it will go back on his step to escape from a dead end
    // And if it can't move at all it will return 4 making it stay put
    if (!checkIfBanned(m_bannedDownCol, m_antLocation.col) && m_lastMove != 0 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 2))) {
        m_bannedDownCol.clear();
        m_lastMove = 2;
        return 2;
    }
    else if (!checkIfBanned(m_bannedLeftRow, m_antLocation.row) && m_lastMove != 1 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 3))) {
        m_bannedLeftRow.clear();
        m_lastMove = 3;
        return 3;
    }
    else if (!checkIfBanned(m_bannedUpCol, m_antLocation.col) && m_lastMove != 2 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 0))) {
        m_bannedUpCol.clear();
        m_lastMove = 0;
        return 0;
    }
    else if (!checkIfBanned(m_bannedRightRow, m_antLocation.row) && m_lastMove != 3 && !checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 1))) {
        m_bannedRightRow.clear();
        m_lastMove = 1;
        return 1;
    }
    else if (!checkInList(nextsAntsLocation, currentAntsLocation, r_state.getLocation(m_antLocation, 2))) {
        m_lastMove = 2;
        return 2;
    }
    else {
        return 4;
    }
}

void AntLogic::refreshPosition(int nextDirection, State& r_state) {
    // I update the position of the ant depending on the move inserted
    m_antLocation = r_state.getLocation(m_antLocation, nextDirection);
}
