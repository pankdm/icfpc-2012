#pragma once

#include "board.h"
#include "getch.h"
#include "clear_screen.h"

class MakeMoves
{
public:
    MakeMoves(const TBoard& board, const vector<EMove>& moves)
        : Board(board)
        , Moves(moves)
    { }

    void Show() {
        TBoard startBoard(Board);
        for (int i = 0; i < sz(Moves); ++i) {
            cout << Board.ToString() << endl;
            char c = getch();
            printf ("You pressed: %c\n", c);
            if (c == 'q') break;
            Board.MakeMove(Moves[i]);
        }
        cout << Board.ToString() << endl;
    }

private:
    TBoard Board;
    vector<EMove> Moves;
};

