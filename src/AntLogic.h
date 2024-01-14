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
    vector<int> bannedLeftRow;
    vector<int> bannedRightRow;
    vector<int> bannedUpCol;
    vector<int> bannedDownCol;


public:
    AntLogic(Location Antloc, Location objectivLoc, boolean warriorOn) // Création de la variable lorsqu'elle est le debut et qu'elle n'a donc pas de position précédente
    {
        AntLocation = Antloc;
        Objectif = objectivLoc;
        Warrior = warriorOn;
    };

    int GetNextMove(Location newObjectivLoc, vector<vector<Square>> gameStatGrid) {
        int d1 = AntLocation.row - Objectif.row;
        int d2 = AntLocation.col - Objectif.col;
        checkAndAddPositionToBanList(gameStatGrid);
        if (Objectif == newObjectivLoc) {
            decisionMaking(d1, d2);
        }
        else {
            clearAllList();
            decisionMaking(d1, d2);
        }
    };

    boolean checkIfBanned(vector<int> testedList, int testedInt) {
        boolean isBanned = false;
        for (int i = 0; i < testedList.size(); i++) {
            if (testedInt == testedList[i]) {
                isBanned = true;
            }
        }
        return isBanned;
    };

    void checkAndAddPositionToBanList(vector<vector<Square>> gameStatGrid) {
        // Function Checking if the column or row is water and is not already in this banned list and if so add it to the ban list
        if (gameStatGrid[AntLocation.row - 1][AntLocation.col].isWater && !checkIfBanned(bannedUpCol, AntLocation.col)) {
            bannedUpCol.insert(bannedUpCol.begin(), AntLocation.col);
        }
        if (gameStatGrid[AntLocation.row + 1][AntLocation.col].isWater && !checkIfBanned(bannedDownCol, AntLocation.col)) {
            bannedDownCol.insert(bannedDownCol.begin(), AntLocation.col);
        }
        if (gameStatGrid[AntLocation.row][AntLocation.col - 1].isWater && !checkIfBanned(bannedLeftRow, AntLocation.row)) {
            bannedLeftRow.insert(bannedLeftRow.begin(), AntLocation.row);
        }
        if (gameStatGrid[AntLocation.row][AntLocation.col + 1].isWater && !checkIfBanned(bannedRightRow, AntLocation.row)) {
            bannedRightRow.insert(bannedRightRow.begin(), AntLocation.row);
        }
    };

    void clearAllList() {
        bannedUpCol.clear();
        bannedLeftRow.clear();
        bannedDownCol.clear();
        bannedRightRow.clear();
    };

    int decisionMaking(int d1, int d2) {
        if (d1 > 0) {
            // L'objectif se trouve vers le nord par rapport à ma 
            if (!checkIfBanned(bannedUpCol, AntLocation.col)) {
                // Si la colone n'est pas dans les colones a éviter Et que la fourmie n'est PAS DESCENDU au tour d'avant alors la fourmit va vers le NORD
                bannedUpCol.clear();
                AntLocation.col = AntLocation.col - 1;
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row)) {
                bannedLeftRow.clear();
                AntLocation.row = AntLocation.row - 1;
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col)) {
                bannedDownCol.clear();
                AntLocation.col = AntLocation.col + 1;
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row)) {
                bannedRightRow.clear();
                AntLocation.row = AntLocation.row + 1;
                return 1;
            }
            else {
                return 0;
            }
        }

        else if (d2 > 0) {
            if (!checkIfBanned(bannedLeftRow, AntLocation.row)) {
                bannedLeftRow.clear();
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col)) {
                bannedDownCol.clear();
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row)) {
                bannedRightRow.clear();
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col)) {
                bannedUpCol.clear();
                return 0;
            }
            else {
                return 3;
            }
        }

        else if (d1 < 0) {
            if (!checkIfBanned(bannedDownCol, AntLocation.col)) {
                bannedDownCol.clear();
                return 2;
            }
            else if (!checkIfBanned(bannedRightRow, AntLocation.row)) {
                bannedRightRow.clear();
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col)) {
                bannedUpCol.clear();
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row)) {
                bannedLeftRow.clear();
                return 3;
            }
            else {
                return 2;
            }
        }

        else if (d2 < 0) {
            if (!checkIfBanned(bannedRightRow, AntLocation.row)) {
                bannedRightRow.clear();
                return 1;
            }
            else if (!checkIfBanned(bannedUpCol, AntLocation.col)) {
                bannedUpCol.clear();
                return 0;
            }
            else if (!checkIfBanned(bannedLeftRow, AntLocation.row)) {
                bannedLeftRow.clear();
                return 3;
            }
            else if (!checkIfBanned(bannedDownCol, AntLocation.col)) {
                bannedDownCol.clear();
                return 2;
            }
            else {
                return 1;
            }
        }
    };
};