#pragma once

#include "board.h"
#include "sandbox.h"
#include "solution.h"

volatile static int STOP_ALL_ITERATIONS = 0;

class TPathAnnealing {
    TBoard Board;
    set<TPosition> Lambdas;
    vector<TPosition> Teleports;
    vector<TPosition> CurrentPath, BestPath, NewPath;
    int CurrentScore, BestScore, NewScore;
    int Iters;
    TSandBox SandBox;

    void Init();

public:
    TPathAnnealing(const TBoard &board)
        : Board(board)
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
    vector<TPosition> AddRandomWaypoint(const vector<TPosition>& path);
    vector<TPosition> DeleteRandomWaypoint(const vector<TPosition>& path);
    vector<TPosition> AddRandomTeleport(const vector<TPosition> &path);
    vector<TPosition> RandomTranspose(const vector<TPosition>& path);
    //vector<TPosition> AddRandomStone(const vector<TPosition>& path);
    vector<TPosition> ShuffleAll(const vector<TPosition>& path);
};

class TAnnealSolution: public IDebugSolution {
    vector<EMove> Moves;
    size_t CurrentMove;
    int Iters;

public:
    TAnnealSolution(const int iters = 1000)
        : Iters(iters)
    {
    }

    virtual void Solve(const TBoard &board, vector<EMove> *moves) {
        TPathAnnealing annealing(board);
        annealing.DoIterations(Iters);
        *moves = Moves = annealing.GetBestMoves();
        CurrentMove = 0;
    }

    virtual EMove NextMove() {
        return CurrentMove < Moves.size() ? Moves[CurrentMove++] : Abort;
    }

    virtual TPosition GetTarget() const {
        return TPosition();
    }
    virtual string GetInfo() const {
        return "";
    }
};
