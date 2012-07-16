#include "multisolver.h"
#include "scores.h"
#include <algorithm>

void MultiSolver::Solve(const TBoard &board, vector<EMove> *bestMoves) {
    vector<EMove> moves;
    int bestScore = -INF;
    Scores.resize(Solutions.size());
    Times.resize(Solutions.size());
    for (size_t sn = 0; sn < Solutions.size(); ++sn) {
        float startTime = static_cast<float>(clock());
        Solutions[sn]->Solve(board, &moves);
        Scores[sn] = CountTotalScores(board, moves);
        Times[sn] = (clock() - startTime) / CLOCKS_PER_SEC;
        if (Scores[sn] > bestScore) {
            bestScore = Scores[sn];
            *bestMoves = moves;
        }
    }
}

void MultiSolver::PrintSolversInfo() {
    cerr << "Best result is " << (*max_element(Scores.begin(), Scores.end())) << endl;
    for (size_t sn = 0; sn < Solutions.size(); ++sn) {
        cerr << "\t#" << sn << ":   Score is " << Scores[sn] << ". Time is " << Times[sn] << endl;
    }
}
