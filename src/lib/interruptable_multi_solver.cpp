#include "interruptable_multi_solver.h"
#include "annealing.h"

vector<EMove> TIntMultiSolver::Solve(const TBoard& board)
{
    Board = board;
    for (int i = 0; i <sz(Solutions); ++i) {
        try {
            if (STOP_ALL_ITERATIONS) break;
            vector<EMove> moves;
            Solutions[i]->Solve(board, &moves);
            Results.push_back(moves);

//            int score = CountTotalScores(board, moves);
//            ShowMoves(moves);
//            cerr << "Total moves: " << sz(moves) << endl;
//            cout << "Total score: " << score << endl;
        } catch(...) {
//        cout << "catched exception" << endl;
            continue;
        }
    }
}

vector<EMove> TIntMultiSolver::GetBestnAnswer()
{
    int bestScore = -INF;
    vector<EMove> bestMoves;
    for (int i = 0; i < sz(Results); ++i) {
        vector<EMove> moves;
        int currentScore = CountTotalScores(Board, Results[i], &moves);
        if (currentScore > bestScore) {
            swap(bestMoves, moves);
            bestScore = currentScore;
        }
    }
    return bestMoves;
}

