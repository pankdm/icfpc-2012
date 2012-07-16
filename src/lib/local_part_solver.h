#pragma once

#include "position.h"
#include "pathfinder.h"
#include "board.h"
#include "common.h"
#include "solution.h"

vector<EMove> solveForLocalPart(const vector<TPosition>& part,
                                TBoard board,
                                const TPositionsV& in,
                                const TPositionsV& out,
                                bool outIsList = false);

class TLocalSolver:
    public IDebugSolution
{
public:
    virtual EMove NextMove() {
        if (CurrentMove < sz(Ans)) {
            EMove move = Ans[CurrentMove];
            ++CurrentMove;
            return move;
        } else {
            return Abort;
        }
    }

    virtual void Solve(const TBoard &board, vector<EMove> *moves)
    {
        Board = board;
        CurrentMove = 0;

        TPositionsV in(1, board.GetRobotPosition());
        TPositionsV out(1, board.GetLiftPosition());
        TPositionsV positions;
        for (int i = 0; i < board.XMax; ++i)
            for (int j = 0; j < board.YMax; ++j) {
                positions.push_back(TPosition(i,j));
            }

        Ans = solveForLocalPart(positions, board, in, out);
        *moves = Ans;
    }

    virtual string GetInfo() const
    {
        return "";
    }

    virtual TPosition GetTarget() const
    {
        return TPosition();
    }
private:
    TBoard Board;
    vector<EMove> Ans;
    int CurrentMove;
};