#pragma once

#include "board.h"
#include "position.h"

vector<TPosition> randomCenters(const TBoard &board, int parts);
vector< vector<TPosition> > splitToParts(const TBoard& board, int partsNum, const TPositionsV& centers, vector<vector<int> >* marks);
void veiwParts(const TBoard& board, int partsNum);
vector<TPosition> regularCenters(const TBoard &board, int partsNum);

struct RandomCentersGenerator{
    TPositionsV operator()(const TBoard &board, int partsNum) const {
        return randomCenters(board, partsNum);
    }
};

class Partition{
public:
    Partition() {
    }
    template<class T>
    void build(const TBoard& board, int partsNum, T centersGenerator) {
        centers = centersGenerator(board, partsNum);
        //centers = RandomCentersGenerator()(board, partsNum);
        debug(centers);
        parts = splitToParts(board, partsNum, centers, &marks);
        debug(parts);
    }
    int getMark(int x, int y) const {
        return marks[x][y];
    }
    vector< vector<TPosition> > parts;
    TPositionsV centers;
private:
    vector< vector<int> > marks;
};
