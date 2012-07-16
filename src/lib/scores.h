#pragma once

#include "board.h"

int CountTotalScores(const vector<string>& input, const string& moves);
int CountTotalScores(TBoard board, const vector<EMove> &moves, vector<EMove>* cutMoves = NULL);

