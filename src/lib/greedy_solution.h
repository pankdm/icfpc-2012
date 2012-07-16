#pragma once

#include "solution.h"
#include "board.h"
#include "greedy_solution.h"

class TGreedySolution
    : public IDebugSolution {
private:
	TBoard Board;

	vector<EMove> MoveBuffer;
    int MoveId;

	vector<string> Info;
    vector<TPosition> Targets;

	bool FindNextTarget(
		const TPosition& currentPosition,
		TPosition& answer);
	
	void Init();
    set<TPosition> UnreachableTarget;

public:
    void Solve(const TBoard &board, vector<EMove> *moves)
	{
        Board = board;
        Init();
        *moves = MoveBuffer;
	}

    virtual EMove NextMove();
    virtual string GetInfo() const;
    virtual TPosition GetTarget() const;
};
