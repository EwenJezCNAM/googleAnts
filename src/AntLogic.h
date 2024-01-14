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
    Location AntLocation;
private:
    boolean Warrior;
    Location Objectif;
    int lastMove;
    vector<int> bannedLeftRow;
    vector<int> bannedRightRow;
    vector<int> bannedUpCol;
    vector<int> bannedDownCol;


public:
    AntLogic(Location _Antloc, Location _objectivLoc, boolean _warriorOn) // Création de la variable lorsqu'elle est le debut et qu'elle n'a donc pas de position précédente
    {
        AntLocation = _Antloc;
        Objectif = _objectivLoc;
        Warrior = _warriorOn;
    };

    int GetNextMove(Location newObjectivLoc, State& state, vector<Location> NextsAntsLocation) {
        int d1 = AntLocation.row - Objectif.row;
        int d2 = AntLocation.col - Objectif.col;
        checkAndAddPositionToBanList(state);
        if (Objectif == newObjectivLoc) {
            int decision = decisionMaking(d1, d2, state, NextsAntsLocation);
            return decision;
        }
        else {
            clearAllList();
            Objectif = newObjectivLoc;
            int decision = decisionMaking(d1, d2, state, NextsAntsLocation);
            return decision;
        }
    };

    boolean checkIfBanned(vector<int> testedList, int testedInt) {
        boolean isBanned = false;
        for (int i = 0; i < testedList.size(); i++) {
            if (testedInt == testedList[i]) {
                isBanned = true;
                break;
            }
        }
        return isBanned;
    };

    boolean checkInList(vector<Location> testedList, Location testedInt) {
        boolean isInList = false;
        for (int i = 0; i < testedList.size(); i++) {
            if (testedInt == testedList[i]) {
                isInList = true;
                break;
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
        bannedUpCol.clear();
        bannedLeftRow.clear();
        bannedDownCol.clear();
        bannedRightRow.clear();
    };

    int decisionMaking(int d1, int d2, State& state, vector<Location> NextsAntsLocation) {
        if (d1 > 0) {
            // L'objectif se trouve vers le nord par rapport à ma 
            if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 0))){
                // Si la colone n'est pas dans les colones a éviter Et que la fourmie n'est PAS DESCENDU au tour d'avant alors la fourmit va vers le NORD
                bannedUpCol.clear();
                lastMove = 0;
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 3))) {
                bannedLeftRow.clear();
                lastMove = 3;
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 2))) {
                bannedDownCol.clear();
                lastMove = 2;
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 1))) {
                bannedRightRow.clear();
                lastMove = 1;
                return 1;
            }
            else if (!checkInList(NextsAntsLocation, state.getLocation(AntLocation, 0))){
                lastMove = 0;
                return 0;
            }
            else {
                return 4;
            }
        }

        else if (d2 > 0) {
            if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 3))) {
                bannedLeftRow.clear();
                lastMove = 3;
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 2))) {
                bannedDownCol.clear();
                lastMove = 2;
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 1))) {
                bannedRightRow.clear();
                lastMove = 1;
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 0))) {
                bannedUpCol.clear();
                lastMove = 0;
                return 0;
            }
            else if (!checkInList(NextsAntsLocation, state.getLocation(AntLocation, 3))){
                lastMove = 3;
                return 3;
            }
            else {
                return 4;
            }
        }

        else if (d1 < 0) {
            if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 2))) {
                bannedDownCol.clear();
                lastMove = 2;
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 1))) {
                bannedRightRow.clear();
                lastMove = 1;
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 0))) {
                bannedUpCol.clear();
                lastMove = 0;
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 3))) {
                bannedLeftRow.clear();
                lastMove = 3;
                return 3;
            }
            else if (!checkInList(NextsAntsLocation, state.getLocation(AntLocation, 2))){
                lastMove = 2;
                return 2;
            }
            else {
                return 4;
            }
        }

        else if (d2 < 0) {
            if (!checkIfBanned(bannedRightRow, AntLocation.row) && lastMove != 3 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 1))) {
                bannedRightRow.clear();
                lastMove = 1;
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col) && lastMove != 2 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 0))) {
                bannedUpCol.clear();
                lastMove = 0;
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row) && lastMove != 1 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 3))) {
                bannedLeftRow.clear();
                lastMove = 3;
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col) && lastMove != 0 && !checkInList(NextsAntsLocation, state.getLocation(AntLocation, 2))) {
                bannedDownCol.clear();
                lastMove = 2;
                return 2;
            }
            else if (!checkInList(NextsAntsLocation, state.getLocation(AntLocation, 1))) {
                lastMove = 1;
                return 1;
            }
            else {
                return 4;
            }
        }
    };

    Location refreshPosition(int nextDirection, State& state) {
        AntLocation = state.getLocation(AntLocation, nextDirection);
        return AntLocation;
    }
};