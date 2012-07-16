#include "../lib/board.h"
#include "../lib/position.h"
#include "../lib/common.h"
#include "../lib/getch.h"
#include "../lib/clear_screen.h"
#include "../lib/scores.h"
#include "../lib/board_reader.h"

int main(int argc, char** argv) {
    cout << argv[1] << endl;
    TBoard board = ReadBoardFromFile(string(argv[1]));
    board.DebugMode = true;

    string input(argv[2]);
    vector<EMove> moves;

    cout << board.ToString() << endl;
    for (int i = 0; i < sz(input); ++i) {
        EMove dir = MoveFromChar(input[i]);

        board.MakeMove(dir);
        cout << board.ToString() << endl;
        moves.push_back(dir);
    }

    TBoard board2 = ReadBoardFromFile(string(argv[1]));
    int score = CountTotalScores(board2, moves);
    string ans = ConvertToAnswer(moves);
    cout << "Answer: " << ans << endl;
    cout << "Total moves: " << sz(ans) << endl;
    cout << "Total score: " << score << endl;

    return 0;
}




