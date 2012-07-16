
#include "scores.h"
#include "position.h"

int CountTotalScores(const vector<string> &input, const string& moves) {
    TBoard board(input);
    vector<EMove> eMoves(moves.size());
    for (size_t i = 0; i < moves.size(); ++i) {
        eMoves[i] = MoveFromChar(moves[i]);
    }
    return CountTotalScores(board, eMoves);
}

int CountTotalScores(TBoard board, const vector<EMove> &moves, vector<EMove>* cutMoves)
{
    int scores = 0;
    int totalLambda = board.LambdasCount;
    for (int i = 0; i < sz(moves); ++i) {
        board.MakeMove(moves[i]);
        if (moves[i] == Abort) break;
        --scores;
        if (cutMoves != NULL) {
            if (board.GetGameState() == Losing) {
                cutMoves->push_back(Abort);
            } else {
                cutMoves->push_back(moves[i]);
            }
        }

        if (board.GetGameState() != Running) {
            break;
        }
    }

    int lambdasGet = totalLambda - board.LambdasCount;
    scores += lambdasGet * 25;

    EState state = board.GetGameState();
    if (state == Winning) {
        scores += lambdasGet * 50;
    }
    if (state == Aborted) {
        scores += lambdasGet * 25;
    }

    return scores;
}

