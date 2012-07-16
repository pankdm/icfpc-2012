#include "../lib/tester.h"
#include "../lib/greedy_solution.h"
#include "../lib/solution.h"
#include "../lib/annealing.h"
#include "../lib/multisolver.h"
#include "../lib/solver_by_split_and_exact.h"

int main(int argc, char *argv[]) {
    string algoType = "anneal";
    if (argc < 2) {
        cout << "Usage: \n\ttesting.exe file_name [algo_type(anneal|local)]" << endl;
        return 0;
    }
    if (argc >= 3)
        algoType = argv[2];
    string fileName = argv[1];
    //ISolution* solution = new TAnnealSolution(10000);
    //for (int p = 3; p < 10; ++p) {
    ISolution* solution;
    if (algoType == "anneal")
        solution = new TAnnealSolution(5000);
    else if (algoType == "local") {
        int p = ToInt(string(argv[3]));
        solution = new TLocalSolution(p, "regular");
    }
    
    Runner tester;
    
    // Path to files in the fileName should be absolute or be specified from the running directory
    tester.RunAll(solution, fileName); // Solution + file with list of maps
    //}
/*
    MultiSolver solver;
    solver.AddSolution(new TAnnealSolution(10000));
    solver.AddSolution(new TGreedySolution());

    TBoard board = ReadBoardFromFile("contest2.map");
    vector<EMove> moves;
    solver.Solve(board, &moves);
    solver.PrintSolversInfo();
*/
    return 0;
}
