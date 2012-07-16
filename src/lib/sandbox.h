#pragma once

#include "board.h"

class TSandBox {
    TBoard Board;
    vector<vector<int> > Field;

public:
    TSandBox(const TBoard &board)
        : Board(board)
        , Field(board.YMax, vector<int>(board.XMax))
    {
    }
    int TryTargetSequence(const vector<TPosition> &targets);
    int TryTargetSequence(const vector<TPosition> &targets, vector<EMove> &moves);
};
