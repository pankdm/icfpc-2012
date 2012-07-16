#include "local_part_solver.h"
#include "local_annealing.h"
#include "scores.h"

vector<EMove> path(const TPosition& f, const TPosition& t, const TBoard& board) {
    vector<EMove> moves;
	TakeShortestPath(board, f, t, moves);
    return moves;
}

int pathLength(const TPosition& f, const TPosition& t, const TBoard& board) {
    vector<EMove> moves;
	if (TakeShortestPath(board, f, t, moves))
        return moves.size();
    return INF;
}

string pathString(const TPosition& f, const TPosition& t, const TBoard& board) {
    return ConvertToAnswer(path(f,t,board));
}

TBoard applyPath(const TPosition& f, const TPosition& t, TBoard board) {
    TBoard nb(board);
    nb.MakeMoves(path(f, t, nb));
    return nb;
} 

vector<EMove> solveForLocalPart(const vector<TPosition>& part, TBoard board, const TPositionsV& in, const TPositionsV& out, bool outIsList) {
    TBoard originBoard(board);
	vector<TPosition> lambdas;
	for (int i = 0; i < part.size(); ++i) {
		if (board.IsLambda(part[i])) {
			lambdas.push_back(part[i]);
		}
	}
    debug(lambdas);
    if (lambdas.size() == 0) {
        return shortestPathFromOneToMany(board, in[0],  out).path;
    }
    if (lambdas.size() > 8 || 1LL * board.XMax * board.YMax > 400) { // if cells > 400 => run TLocalPathAnnealing
        UNREACHABLE();
        /*TLocalPathAnnealing tla(board, part, in[0], out);
        tla.DoIterations(100);
        return tla.GetBestMoves();
        UNREACHABLE("Sorry, too much lambdas");*/
	} else {
		int n = lambdas.size();
		vector<int> enter(n, INF);
		vector<int> enterFrom(n, -1);
		vector<int> exit(n, INF);
		vector<int> exitTo(n, -1);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < in.size(); ++j) {
				if (enter[i] > pathLength(in[j], lambdas[i], board)) {
					enter[i] = pathLength(in[j], lambdas[i], board);
					enterFrom[i] = j;
				}
			}
		}
        //debug(enter);
        //debug(exit);
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < out.size(); ++j) {
				if (exit[i] > pathLength(out[j], lambdas[i], board)) {
					exit[i] = pathLength(out[j], lambdas[i], board);
					exitTo[i] = j;
				}
			}
		}

		vector<vector<int> > dp(1 << n, vector<int> (n, INF));
		vector<vector<int> > prev(1 << n, vector<int> (n, -1));
        vector<vector<TBoard> > dpBoards(1 << n, vector<TBoard> (n, board));

		for (int i = 0; i < n; ++i) {
			if (enter[i] < INF) {
                dp[1 << i][i] = enter[i];
                dpBoards[1 << i][i] = applyPath(in[enterFrom[i]], lambdas[i], board);
            }
		}
        std::cerr << "run mask dp\n";
		for (int mask = 1; mask < (1 << n); ++mask) {
			for (int i = 0; i < n; ++i) {
				if ((mask & (1 << i)) == 0)
					continue;
                for (int j = 0; j < n; ++j) {
                    if (mask & (1 << j))
                        continue;
                    int nmask = mask ^ (1 << j);
                    int add = 0;
                    if (pathLength(lambdas[i], lambdas[j], dpBoards[mask][i]) < INF) {
                        TBoard afterBoard = applyPath(lambdas[i], lambdas[j], dpBoards[mask][i]);
                        //std::cerr << lambdas[i].ToString() << " --> " << lambdas[j].ToString() << "\n";
                        //std::cerr << pathString(lambdas[i], lambdas[j], dpBoards[mask][i]) << "\n";
                        //cerr << dpBoards[mask][i].ToString() << "\n";
                        //cerr << "==========\n";
                        //cerr << afterBoard.ToString() << "\n";

                        if (afterBoard.GetRobotPosition() != lambdas[j])
                            continue;
                        if (nmask + 1 == (1 << n)) {
                            add = pathLength(lambdas[j], out[exitTo[j]], afterBoard);
                            //if (outIsList) {  
                            add = min(INF/2, add);
                            //}
                        //    std::cerr << "LAST\n" << afterBoard.ToString() << "\n";
                        } else {
                            if (afterBoard.GetGameState() == Losing) {
                          //      std::cerr << afterBoard.ToString() << "\n";
                                add = INF;
                            } else {
                                //std::cerr << afterBoard.ToString() << "\n";
                            }
                        }
				        if (dp[nmask][j] > dp[mask][i] + pathLength(lambdas[i], lambdas[j], dpBoards[mask][i]) + add) {
					        dp[nmask][j] = dp[mask][i] + pathLength(lambdas[i], lambdas[j], dpBoards[mask][i]) + add;
                            dpBoards[nmask][j] = applyPath(lambdas[i], lambdas[j], dpBoards[mask][i]);
                            prev[nmask][j] = i;
				        }
                    }
                }
			}
		}
        int bestMask = 0;
        int last = 0;
        for (int mask = (1 << n) - 1; mask >= 0; --mask) {
            int m = INF;
            for (int i = 0 ;i < n; ++i) {
                m = min(m, dp[mask][i]);
            }
            if (m < INF) {
                if (BitCount(mask) > BitCount(bestMask))
                    bestMask = mask;
            }
        }
        debug(bestMask);
        for (int i = 0; i < n; ++i) {
            if (dp[bestMask][i] < dp[bestMask][last])
                last = i;
        }
        if (BitCount(bestMask) == 0) {
            return shortestPathFromOneToMany(board, in[0],  out).path;
        }
        debug(dp[bestMask][last] );
        debug(lambdas);
        debug(BitCount(bestMask));
        //std::cerr << dp[(1 << n) - 1][last] << "--best score\n";
        if (dp[bestMask][last] >= INF) {
        //    UNREACHABLE("not found");
        }
        vector<int> order;
        int mask = bestMask;
        int weExitTo = exitTo[last];
        int weEnterFrom = -1;
        while(mask) {
            order.push_back(last);
            weEnterFrom = enterFrom[last];
            int t = last;
            last = prev[mask][last];
            mask ^= 1 << t;
        }
        reverse(all(order));
        vector<EMove> res;
        Append(res, path(in[weEnterFrom], lambdas[order[0]], board));
        board.MakeMoves(path(in[weEnterFrom], lambdas[order[0]], board));
        ShowMoves(res);
        for (int i = 0; i + 1 < order.size(); ++i) {
            Append(res, path(lambdas[order[i]], lambdas[order[i + 1]], board));
            ShowMoves(res);
            board.MakeMoves(path(lambdas[order[i]], lambdas[order[i + 1]], board));
        }

        Append(res, path(lambdas[order.back()], out[weEnterFrom], board));
        board.MakeMoves(path(lambdas[order.back()], out[weEnterFrom], board));
        debug(CountTotalScores(originBoard, res));
        return res;
	}
    UNREACHABLE("None");
}
