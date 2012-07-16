#include "tester.h"

#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>

int Runner::RunOne(ISolution *solution, const string &oneMapFileName, vector<EMove> *resMoves) {
    int score = 0;
    TBoard board = ReadBoardFromFile(oneMapFileName);
    vector<EMove> moves;
    std::cerr << board.ToString() << endl;
    solution->Solve(board, &moves);
    *resMoves = moves;
    return CountTotalScores(board, moves);
}

void Runner::RunAll(ISolution *solution, const string &allMapsFileName) {
    float startTimeAll = static_cast<float>(clock());
    ifstream inf(allMapsFileName.c_str());
    vector<string> fileNames;
    {
        string line;
        while (inf >> line)
            fileNames.push_back(line);
    }
    cout << "Maps will be read from " << allMapsFileName << endl;
    int totalScore = 0;
    for (size_t fn = 0; fn < fileNames.size(); ++fn) {
        string &fileName = fileNames[fn];
        float startTimeOne = static_cast<float>(clock());
        cout << "\tFile #" << fn << ": " << fileName << " ..." << endl;
        vector<EMove> moves;
        int score = RunOne(solution, fileName, &moves);
        cout << "\t\tScore: " << score << ". Time is " << (clock() - startTimeOne) / CLOCKS_PER_SEC << endl;
        cout << "\t\t" << ConvertToAnswer(moves) << endl;
        totalScore += score;
    }
    cout << "----------------------------------------" << endl;
    cout << "Total score is " << totalScore << ". TotalTotal time is " << (clock() - startTimeAll) / CLOCKS_PER_SEC << endl;
}
