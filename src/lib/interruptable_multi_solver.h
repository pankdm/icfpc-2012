#pragma once

#include "common.h"
#include "move.h"
#include "solution.h"
#include "scores.h"

class TIntMultiSolver {
    vector<ISolution*> Solutions;
    vector<vector<EMove> > Results;
    TBoard Board;
public:
    void AddSolution(ISolution* solution) {
        Solutions.push_back(solution);
    }
    vector<EMove> Solve(const TBoard& board);
    vector<EMove> GetBestnAnswer();
};


