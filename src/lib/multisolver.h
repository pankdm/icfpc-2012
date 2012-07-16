#pragma once

#include "solution.h"

class MultiSolver: public ISolution {
    vector<ISolution*> Solutions;
    vector<float> Times;
    vector<int> Scores;
public:
    void AddSolution(ISolution* solution) {
        Solutions.push_back(solution);
    }
    void Solve(const TBoard& board, vector<EMove> *moves);
    void PrintSolversInfo();
};