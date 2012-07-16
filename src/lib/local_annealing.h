#pragma once

#include "board.h"
#include "sandbox.h"

class TLocalPathAnnealing {
    TBoard Board;
    vector<TPosition> Part;
    TPosition In;
    vector<TPosition> Out;
    set<TPosition> Lambdas;
    vector<TPosition> CurrentPath, BestPath, NewPath;
    int CurrentScore, BestScore, NewScore;
    int Iters;
    TSandBox SandBox;

    void Init();

public:
    TLocalPathAnnealing(const TBoard &board, const vector<TPosition> &part, const TPosition &in, const vector<TPosition> &out)
        : Board(board)
        , Part(part)
        , In(in)
        , Out(out)
        , Iters(0)
        , SandBox(board)
    {
        Init();
    }

    void DoIterations(int itsToDo);
    int GetBestScore() const;
    vector<TPosition> GetBestPath() const;
    vector<EMove> GetBestMoves();

private:
    bool Lucky() const;

    vector<TPosition> MakeRandomChange(const vector<TPosition> &path);
    vector<TPosition> AddRandomWaypoint(vector<TPosition> path);
    vector<TPosition> DeleteRandomWaypoint(vector<TPosition> path);
    vector<TPosition> RandomTranspose(vector<TPosition> path);
    //vector<TPosition> AddRandomStone(vector<TPosition> path);
    vector<TPosition> ChangeFinish(vector<TPosition> path);
    vector<TPosition> ShuffleAll(vector<TPosition> path);
};
