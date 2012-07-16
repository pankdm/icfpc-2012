#include "parts_splitter.h"

vector<TPosition> randomCenters(const TBoard &board, int partsNum) {
    vector<TPosition> passable;
    for (int x = 0; x < board.XMax; ++x) {
        for (int y = 0; y < board.YMax; ++y) {
            if (board.IsPassable(TPosition(x, y))) {
                passable.pb(TPosition(x, y));
            }
        }
    }
    random_shuffle(passable.begin(), passable.end());
    partsNum = min(partsNum, (int)passable.size());
    return vector<TPosition>(passable.begin(), passable.begin() + partsNum);
}

vector<TPosition> regularCenters(const TBoard &board, int partsNum) { 
    TPositionsV centers;
	int xSplits = int(sqrt(1.0 * partsNum));
	int ySplits = partsNum / xSplits;
	int dx = board.XMax / xSplits;
	int dy = board.YMax / ySplits;
	int X, Y;
	X = board.XMax;
	Y = board.YMax;
    for (int i = 0; i < xSplits; ++i) {
		for (int j = 0; j < ySplits; ++j) {
			int cx = dx / 2 + dx * i;
			int cy = dy / 2 + dy * j;
            int x, y;
            y = x = -10000;
            for (int a = dx * i; a != dx * (i + 1); ++a) {
                for (int b = dy * j; b != dy * (j + 1); ++b) {
                    //std::cerr << a << " " << b << "\n";
                    if (board.IsPassable(TPosition(a, b))) {
                        if ((TPosition(x,y)^TPosition(cx, cy)) > (TPosition(a,b)^TPosition(cx,cy))) {
                            x=a;
                            y=b;
                        }
                    }
                }
            }
            if (board.IsPassable(TPosition(x, y))) {
			    centers.push_back(TPosition(x, y)); 
			    //(*marks)[x][y] = centers.size() - 1;
			    //Q.push(TPosition(x, y));
            }
		}
	}
    std::cerr << "Out from regular centers " << centers << "\n";
    return centers;
}

vector< vector<TPosition> > splitToParts(const TBoard& board, int partsNum, const TPositionsV& centers, vector<vector<int> >* marks) {
	// O(m * n)
	int X, Y;
	X = board.XMax;
	Y = board.YMax;
    marks->clear();
    marks->assign(X, vector<int> (Y, -1));
	queue<TPosition> Q;
    for (int i = 0; i < centers.size(); ++i) {
        Q.push(centers[i]);
        (*marks)[centers[i].X][centers[i].Y] = i;
    }
    while(!Q.empty()) {
		TPosition p = Q.front();
		Q.pop();
        vector<TPosition> moves = board.getNeighbours(p);
		for (int i = 0; i < moves.size(); ++i) {
			if (board.IsPassable(moves[i])) {
				int x = moves[i].X;
				int y = moves[i].Y;
				if ((*marks)[x][y] == -1) {
					(*marks)[x][y] = (*marks)[p.X][p.Y];
					Q.push(moves[i]);
				}
			}
		}
	}
	vector<vector<TPosition> > groups(centers.size());
	for (int i = 0; i < marks->size(); ++i)
		for (int j = 0; j < (*marks)[i].size(); ++j)
			if ((*marks)[i][j] >= 0)
				groups[(*marks)[i][j]].push_back(TPosition(i, j));
    //debug(*marks);
	return groups;
}

void veiwParts(const TBoard& board, int partsNum) {
    TPositionsV c;
    vector<vector<int> > m;

    vector<vector<TPosition> > groups = splitToParts(board, partsNum, regularCenters(board, partsNum), &m);
	vector<string> boardsStrings = board.ToStringsVector();
	vector<string> boardsMarkedStrings = board.ToStringsVector();
	//std::cerr << "-------";
	for (int i = 0; i < groups.size(); ++i) {
		for (int j = 0; j < groups[i].size(); ++j) {
			boardsMarkedStrings[groups[i][j].Y][groups[i][j].X] = 'A'  + i;
		}
	}
	for (int i = boardsStrings.size() - 1; i >= 0; --i) {
		std::cerr << boardsStrings[i] << "\t" << boardsMarkedStrings[i] << "\n";
	}
}