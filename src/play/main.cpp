#include "../lib/board.h"
#include "../lib/position.h"
#include "../lib/common.h"
#include "../lib/getch.h"
#include "../lib/clear_screen.h"
#include "../lib/scores.h"
#include "../lib/board_reader.h"

int main(int argc, char** argv) {
    clear_screen();
    cout << argv[1] << endl;
    TBoard board = ReadBoardFromFile(string(argv[1]));
    board.DebugMode = true;

    vector <EMove> moves;
    cout << board.ToString() << endl;
    while(true) {
        char c = getch();
        clear_screen();
        printf ("You pressed: %c\n", c);

        EMove move = MoveLast;
        if (c == 'w') {
            move = Up;
        }
        if (c == 'a') {
            move = Left;
        }
        if (c == 's') {
            move = Down;
        }
        if (c == 'd') {
            move = Right;
        }
        if (c == 'f') {
            move = Wait;
        }
        if (c == 'r') {
            move = Abort;
        }
        if (c == 'c') {
            move = Shave;
        }

        if (move != MoveLast) {
            board.MakeMove(move);
            moves.push_back(move);;
        } else {
            cout << "Nothing changed" << endl;
        }
        cout << board.ToString() << endl;
        if (c == 'q') break;
        if (board.GetGameState() != Running) {
            break;
        }
    }

    TBoard board2 = ReadBoardFromFile(string(argv[1]));
    int score = CountTotalScores(board2, moves);
    string ans = ConvertToAnswer(moves);
    cout << "Answer: " << ans << endl;
    cout << "Total moves: " << sz(ans) << endl;
    cout << "Total score: " << score << endl;



    return 0;
}




