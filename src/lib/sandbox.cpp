#include "sandbox.h"
#include "scores.h"
#include "pathfinder.h"

int TSandBox::TryTargetSequence(const vector<TPosition> &targets) {
    vector<EMove> moves;
    return TryTargetSequence(targets, moves);
}

int TSandBox::TryTargetSequence(const vector<TPosition> &targets, vector<EMove> &moves) {
    TBoard myBoard = Board;
    moves.clear();
    for (size_t i = 0; i < targets.size(); ++i) {
        /*if (!myBoard.IsLambda(targets[i]) && myBoard.GetCellType(targets[i]) != OpenLift) {
            continue;
        }*/
        vector<EMove> path;
        if (!TakeBFS(myBoard, targets[i], &Field, path)) {
            //vector<TPosition> wallsToShift;
            //CalcWallsToRemove(myBoard, targets[i], &wallsToShift);
            if (!TakeBFS(myBoard, targets[i], &Field, path, true)) {
                break;
            } else {
                for (size_t j = 0; j < path.size(); ++j) {
                    moves.push_back(path[j]);
                    myBoard.MakeMove(path[j]);
                }
            }
        } else {
            for (size_t j = 0; j < path.size(); ++j) {
                moves.push_back(path[j]);
                myBoard.MakeMove(path[j]);
            }
        }
    }

    moves.push_back(Abort);
    return CountTotalScores(Board, moves);
}
