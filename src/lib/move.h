#pragma once
#include "common.h"

enum EMove {
    Up,        //
    Down,      //
    Left,      //  do not change this part!
    Right,     //
    Wait,      // 
    Abort,
    Shave,
    MoveLast,
    NotAMove
};

inline EMove Inverse(EMove dir) {
    switch (dir) {
        case Up: return Down;
        case Down: return Up;
        case Left: return Right;
        case Right: return Left;
        default: return dir;
    }
}

inline char CharFromMove(EMove dir) {
    return "UDLRWAS"[dir];
}

inline string ConvertToAnswer(const vector<EMove>& moves)
{
    string res;
    for (int i = 0; i < sz(moves); ++i) {
        res.push_back(CharFromMove(moves[i]));
    }
    return res;
}

inline void ShowMoves(const vector<EMove>& moves)
{
    cerr << "moves: " << ConvertToAnswer(moves) << endl;
}


inline EMove MoveFromChar(char c) {
    switch (c) {
        case 'U': return Up;
        case 'D': return Down;
        case 'L': return Left;
        case 'R': return Right;
        case 'W': return Wait;
        case 'A': return Abort;
        case 'S': return Shave;
        default:
            return Wait;
            fprintf(stderr, "Unexpected char: %c (%d)", c, c);
            assert (false);
    }
    return Wait;
}
