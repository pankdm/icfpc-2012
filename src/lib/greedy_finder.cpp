
#include "position.h"
#include "greedy_finder.h"
#include "pathfinder.h"

#include "getch.h"
#include "clear_screen.h"

bool FindNextTarget(
    const TPosition& currentPosition,
    const TBoard& board,
    TPosition& answer)
{
    bool found = false;
    TPosition result(-1, -1);
    for (int y = 0; y < board.YMax; ++y) {
        for (int x = 0; x < board.XMax; ++x) {
            TPosition next = TPosition(x, y);
            ECellType type = board.GetCellType(next);
            if (type == Lambda) {
                if (!found ||
                    (next ^ currentPosition) < (result ^ currentPosition))
                {
                    result = next;
                    found = true;
                }
            }
            if (type == OpenLift) {
                result = next;
                found = true;
            }
        }
    }
    answer = result;
    return found;
}

string GreedyFind(TBoard board) {
    string ans;
    while (true) {
        char c = getch();
        clear_screen();
        printf ("You pressed: %c\n", c);
        cout << board.ToString() << endl;
        if (c == 'q') break;

        TPosition robotPosition = board.GetRobotPosition();
        if (board.GetGameState() != Running) break;
        TPosition next;
        bool found = FindNextTarget(robotPosition, board, next);
        cerr << "next target is " << next.ToString() << endl;
        if (!found) {
            INFO("didn't found next cell");
            break;
        }

        vector<EMove> moves;
        if (!TakeShortestPath(board, next, moves)) {
        }
        for (int i = 0; i < sz(moves); ++i) {
            board.MakeMove(moves[i]);
            ans.push_back(CharFromMove(moves[i]));
        }

        if (board.GetRobotPosition() != next) {
            INFO("robot now is at " << board.GetRobotPosition().ToString());
            INFO("robot didn't reached goal");
            break;
        }
    }
    return ans;
}
