#pragma once

#include "board.h"

class ISolution {
public:
    virtual void Solve(const TBoard& board, vector<EMove> *moves) = 0;
};


class IDebugSolution: public ISolution {
public:
    virtual EMove NextMove() = 0;
	virtual string GetInfo() const = 0;	
    virtual TPosition GetTarget() const = 0;
};