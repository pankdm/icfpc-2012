#pragma once

#include "solution.h"
#include "board_reader.h"
#include "scores.h"

class Runner {
  public:
    int RunOne(ISolution *solution, const string &oneMapfileName, vector<EMove> *resMoves);
    void RunAll(ISolution *solution, const string &allMapsFileName);    
};