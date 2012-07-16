#include "solver_by_split_and_exact.h"
#include "local_annealing.h"

void buildOrderMyAm(int v, const vector<vector<int> >& am, vector<bool>& used, const vector<int>& lambdasNum, vector<int>* order);

vector<int> determinePathByParts(const Partition& partition, const TBoard& board) {
    int partsNum = partition.parts.size();
    vector<int> lambdasNum(partsNum, 0);
    for (int i = 0; i < partition.parts.size(); ++i) {
        for (int j = 0; j < partition.parts[i].size(); ++j) {
            if (board.IsLambda(partition.parts[i][j]))
                ++lambdasNum[i];
        }
    }
    
    vector<vector<int> > am(partsNum, vector<int> (partsNum, 0));
    for (int i = 0; i < board.XMax; ++i) {
        for (int j = 0; j < board.YMax; ++j) {
            int firstPart = partition.getMark(i, j);
            TPositionsV neighbours = board.getNeighbours(TPosition(i, j));
            for (int k = 0; k < neighbours.size(); ++k) {
                int secondPart = partition.getMark(neighbours[k].X, neighbours[k].Y);
                if (firstPart != secondPart && firstPart >= 0 && secondPart >= 0) {
                    am[firstPart][secondPart] = 1;
                }
            }
        }
    }
    vector<int> order;
    vector<bool> used(partsNum, 0);
    debug(board.GetRobotPosition());
    if (partition.getMark(board.GetRobotPosition().X,board.GetRobotPosition().Y) == -1)
        return order;
    buildOrderMyAm(partition.getMark(board.GetRobotPosition().X,board.GetRobotPosition().Y), am, used, lambdasNum, &order);
    return order;
}

void buildOrderMyAm(int v, const vector<vector<int> >& am, vector<bool>& used, const vector<int>& lambdasNum, vector<int>* order) {
    std::cerr << "in buildOrderMyAm("<<v<<")\n";
    used[v] = 1;
    vector<pair<int, int> > childs;
    for (int i = 0; i < am.size(); ++i) {
        if (!used[i] && am[v][i]) {
            childs.push_back(mp(lambdasNum[i], i));
        }
    }
    sort(all(childs));
    reverse(all(childs));

    order->push_back(v);
    for (int i = 0; i < childs.size(); ++i) {
        buildOrderMyAm(childs[i].second, am, used, lambdasNum, order);
        order->push_back(v);
    }
}

vector<EMove> traverseField(const Partition& partition, const TBoard& board, const vector<int>& order) {
    debug(order);
    vector<EMove> path;
    vector<bool> used(partition.parts.size(), false);
    TBoard curBoard(board);
    for (int i = 0; i < order.size(); ++i) {
        TPositionsV in;
        TPositionsV out;
        in.push_back(curBoard.GetRobotPosition());
        if (i + 1 == order.size()) {
            out.push_back(curBoard.GetLiftPosition());
        } else {
            int partId = order[i];
            for (int j = 0; j < partition.parts[partId].size(); ++j) {
                TPositionsV neighbours = curBoard.getNeighbours(partition.parts[partId][j]);
                for (int k = 0; k < neighbours.size(); ++k) {
                    int nx = neighbours[k].X;
                    int ny = neighbours[k].Y;
                    bool needToAdd = out.empty() || out.back() != partition.parts[partId][j];
                    if (partition.getMark(nx, ny) == order[i + 1] && needToAdd) {
                        out.push_back(partition.parts[partId][j]);
                    }
                }
            }
        }
        vector<EMove> addition;
        std::cerr << "Traverse " << char('A' + order[i]);
        if (i + 1 < order.size()) {
            cerr << " to " << char('A' + order[i + 1]);
        }
        cerr << "\n";
        debug(in[0]);
        debug(out);
        if (used[order[i]]) {
            std::cerr << "...by shortestPathFromOneToMany\n";
            ShortestPathResult shortestPathResult = shortestPathFromOneToMany(curBoard, in[0], out);
            //REQUARE(shortestPathResult.found, "path between parts not found :(");
            addition = shortestPathResult.path;
        } else {
            TPositionsV lambdas = board.GetLambdasPositions();
            if (lambdas.size() > 8 || 1LL * board.XMax * board.YMax > 400) {
                // if cells > 400 => run TLocalPathAnnealing
                std::cerr << "...by TLocalPathAnnealing\n";
                TLocalPathAnnealing tla(board, partition.parts[order[i]], in[0], out);
                tla.DoIterations(100);
                addition = tla.GetBestMoves();
            } else {
                std::cerr << "...by solveForLocalPart\n";
                addition = solveForLocalPart(partition.parts[order[i]], curBoard, in, out, i + 1 == order.size());
            }
            used[order[i]] = true;
        }
        debug(addition);
        curBoard.MakeMoves(addition);
        debug(curBoard.ToString());
        Append(path, addition);
    }
    return path;
};
