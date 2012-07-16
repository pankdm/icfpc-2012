#include "../lib/tester.h"
#include "../lib/greedy_solution.h"
#include "../lib/solution.h"
#include "../lib/annealing.h"
#include "../lib/interruptable_multi_solver.h"
#include "../lib/solver_by_split_and_exact.h"
#include "../lib/board_reader.h"

#ifdef unix
#include <signal.h>
#include <errno.h>
#include <string.h>
#endif

TIntMultiSolver solver;

static void SigIntHandler(int signum) {
//    cout << "here" << endl;
    cout << ConvertToAnswer(solver.GetBestnAnswer()) << endl;
    exit(0);
//    throw exception();
}

void CatchIntSignal()
{
#ifdef unix
    struct sigaction intAction;
    intAction.sa_handler = SigIntHandler;
    sigemptyset(&intAction.sa_mask);
    intAction.sa_flags = SA_RESTART;
    if(sigaction(SIGINT, &intAction, NULL) != 0)
        fprintf(stderr, "sigaction() failed: %s\n", strerror(errno));
#endif
}

vector<int> GetNumIterationsForAnneal() {
    vector<int> ni;
    for (int i = 0; i < 10; ++i) {
        ni.push_back(10 + i * 10);
    }
    for (int i = 0; i < 4; ++i) {
        ni.push_back(100 + i * 50);
    }

    for (int i = 4; i < 10; ++i) {
        ni.push_back(( 1 << i));
    }

    for (int i = 0; i < 5; ++i) {
        ni.push_back(1000 + 2000 * i);
    }

    for (int i = 0; i < 100; ++i) {
        ni.push_back(10000 + 3000 * i);
    }

    for (int i = 13; i < 25; ++i) {
        ni.push_back( 1 << i);
    }
    return ni;
}

int main(int argc, char *argv[]) {
    CatchIntSignal();

    solver.AddSolution(new TGreedySolution());

    // add Anneal;
    vector<int> ni = GetNumIterationsForAnneal();
//    cout << ni << endl;
    for (int i = 0; i < sz(ni); ++i) {
        solver.AddSolution(new TAnnealSolution( ni[i] ));
    }

    TBoard board;
    board = ReadBoardFromStdin();

    solver.Solve(board);
    cout << ConvertToAnswer(solver.GetBestnAnswer()) << endl;

    return 0;
}
