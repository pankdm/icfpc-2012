#pragma once
#include "parts_splitter.h"
#include "move.h"
#include "local_part_solver.h"

vector<int> determinePathByParts(const Partition& partition, const TBoard& board);

vector<EMove> traverseField(const Partition& partition, const TBoard& board, const vector<int>& order);

// for testing
class TLocalSolution: public ISolution {
    vector<EMove> Moves;
    size_t CurrentMove;
    int Parts;
    string CenterGenerationType;
public:
    TLocalSolution(const int parts = 1, const string& centerGenerationType = "regular" )
        : Parts(parts),
          CenterGenerationType(centerGenerationType)
    {
    }

    virtual void Solve(const TBoard &board, vector<EMove> *moves) {
        Partition partition;
        if (CenterGenerationType == "random") {
            partition.build(board, Parts, randomCenters);
        } else if (CenterGenerationType == "regular") {
            partition.build(board, Parts, regularCenters);
        } else {
            UNREACHABLE("wrong centerGenerationType");
        }
        vector<int> order = determinePathByParts(partition, board);
        debug(order);
        vector<EMove> path = traverseField(partition, board, order);
        *moves = path;
    }
};