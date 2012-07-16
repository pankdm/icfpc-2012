#include "board.h"

class TPartsTraverser {
    TBoard *Board;
    int CurrentPart, PartsCount;
    vector< vector<TPosition> > Parts;
    vector< vector<int> > PartsLinks;
    vector<int> LambdasCount;
    vector<double> PartsEval;

public:
    TPartsTraverser(TBoard *board, const vector< vector<TPosition> > &parts)
        : Board(board)
        , PartsCount((int)parts.size())
        , Parts(parts)
    {
    }

    void ReevalParts(int depth);
    void RecountLambdas(int part);
    void MoveToBestPart();
};
