#pragma once

#include "common.h"
#include "board.h"

struct ShortestPathResult {
    vector<EMove> path;
    bool found;
};

bool TakeShortestPath(
	const TBoard &board,
    const TPosition &from,
    const TPosition &dest,
    vector<EMove> &moves,
    bool shortest = true,
    bool ignoreStones = false);

bool TakeShortestPath(const TBoard &board, const TPosition &dest, vector<EMove> &moves, bool shortest = true, bool ignoreStones = false);
bool TakeBFS(const TBoard &board, const TPosition &dest, vector< vector<int> > *distance, vector<EMove> &moves, bool ignoreStones = false);
void CalcWallsToRemove(const TBoard &board, const TPosition &dest, vector<TPosition> *walls);

ShortestPathResult shortestPathFromOneToMany(const TBoard &board,  const TPosition &from, 
        const TPositionsV& destinations, bool shortest = true, bool ignoreStones = false);
