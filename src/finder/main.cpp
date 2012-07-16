
#include "../lib/board.h"
#include "../lib/common.h"
#include "../lib/getch.h"
#include "../lib/clear_screen.h"
#include "../lib/greedy_finder.h"
#include "../lib/scores.h"
#include "../lib/board_reader.h"

#include "../lib/greedy_solution.h"
#include "../lib/local_part_solver.h"

class TVisualizer
{
public:
    TVisualizer(IDebugSolution* solution, const TBoard& board)
        : Solution(solution)
        , Board(board)
    {
        Solution->Solve(Board, &Moves);
    }

    bool DoUntilNextTarget()
    {
        while (true) {
            TPosition nextTarget = Solution->GetTarget();
            if (nextTarget != CurrentTarget) {
                CurrentTarget = nextTarget;
                break;
            }
            if (!DoTurn()) return false;
        }
        return true;
    }

    bool DoTurn()
    {
        EMove nextMove = Solution->NextMove();
        Moves.push_back(nextMove);
        Board.MakeMove(nextMove);
        return (Board.GetGameState() == Running);
    }

    void PrintState()
    {
        cout << Board.ToString() << endl;
        cout << Solution->GetInfo() << endl;
    }

    // readonly
    vector<EMove> Moves;

private:
    IDebugSolution* Solution;
    TBoard Board;

    TPosition CurrentTarget;
};

IDebugSolution* CreateSolution(string type)
{
    if (type == "greed") {
        return new TGreedySolution();
    } else if (type == "local")  {
        return new TLocalSolver();
    } else {
        UNREACHABLE("unknown type");
    }
}

int main(int argc, char** argv) {
    string filename(argv[1]);

    string type = "greed";
    if (argc > 2) {
        type = string(argv[2]);
    }

    TBoard board = ReadBoardFromFile(filename);

    IDebugSolution* solution = CreateSolution(type);

    TVisualizer visualizer(solution, board);
    while (true) {
        clear_screen();
        visualizer.PrintState();
        char c = getch();
        printf ("You pressed: %c\n", c);
        if (c == 'q') break;
        if (c == 'v') {
            if (!visualizer.DoTurn()) break;
        }
        if (c == 'c') {
            if (!visualizer.DoUntilNextTarget()) break;
        }
    }
    visualizer.PrintState();

    string ans;
    const vector<EMove>& moves = visualizer.Moves;
    for (int i = 0; i < sz(moves); ++i) {
        ans.push_back(CharFromMove(moves[i]));
    }

    int score = CountTotalScores(board, moves);
    cout << "Answer: " << ans << endl;
    cout << "Total moves: " << sz(ans) << endl;
    cout << "Total score: " << score << endl;

    delete solution;

    return 0;
}




