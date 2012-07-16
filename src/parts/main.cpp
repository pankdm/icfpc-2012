#include "../lib/board.h"
#include "../lib/common.h"
#include "../lib/getch.h"
#include "../lib/clear_screen.h"
#include "../lib/board_reader.h"
#include "../lib/parts_splitter.h"
#include "../lib/local_part_solver.h"
#include "../lib/solver_by_split_and_exact.h"
#include "../lib/make_moves.h"
#include "../lib/scores.h"

int main(int argc, char** argv) {
    //clear_screen();
    //cout << argv[1] << endl;
    TBoard board = ReadBoardFromFile(string(argv[1]));
    stringstream o;
    o << string(argv[2]);
    int PARTS;
    o >> PARTS;

    debug(board.ToString());
	veiwParts(board, PARTS);
    /*TPositionsV in(1, board.GetRobotPosition());
    TPositionsV out(1, board.GetLiftPosition());
    TPositionsV positions;
    for (int i = 0; i < board.XMax; ++i)
        for (int j = 0; j < board.YMax; ++j) {
            positions.push_back(TPosition(i,j));
        }
    cerr << board.ToString() << "\n";*/
    //ShowMoves(solveForLocalPart(positions, board, in, out, true));
    Partition partition;
    partition.build(board, PARTS, regularCenters);
    vector<int> order = determinePathByParts(partition, board);
    vector<EMove> path = traverseField(partition, board, order);
    ShowMoves(path);
    MakeMoves mm(board, path);
    //mm.Show();
    cerr << "Scores = " << CountTotalScores(board, path) << "\n";
	return 0;
}
