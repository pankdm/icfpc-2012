#include "pathfinder.h"
#include <queue>

bool TakeShortestPath(const TBoard &board,
    const TPosition &from,
    const TPosition &dest,
    vector<EMove> &moves,
    bool shortest,
    bool ignoreStones)
{
    set< pair< int, TPosition > > q;
    TPosition myPos = TPosition(from);
    q.insert(mp(dest ^ myPos, myPos));
    map<TPosition, EMove> previous;
    map<TPosition, int> distance;
    distance[myPos] = 0;
    while (!q.empty()) {
        int currentW = q.begin()->first;
        TPosition currentPos = q.begin()->second;
        q.erase(q.begin());
        if (currentW != distance[currentPos] + (currentPos ^ dest)) {
            continue;
        }
        if (distance.find(dest) != distance.end() && (currentW > distance[dest] || !shortest)) {
            break;
        }
        int currentM = currentW - (currentPos ^ dest);
        for (int dir = Up; dir != Wait; ++dir) {
            TPosition newPos = currentPos.Move(EMove(dir));
            if (!(board.IsPassable(newPos) || (ignoreStones && board.IsValidPosition(newPos) && board.GetCellType(newPos) == Rock))) {
                continue;
            }
            if (distance.find(newPos) == distance.end()) {
                distance[newPos] = INF;
            }
            if (currentM + 1 < distance[newPos]) {
                previous[newPos] = EMove(dir);
                q.erase(mp(distance[newPos] + (newPos ^ dest), newPos));
                distance[newPos] = currentM + 1;
                q.insert(mp(currentM + 1 + (newPos ^ dest), newPos));
            }
        }
    }

    if (distance.find(dest) == distance.end()) {
        return false;
    }
    TPosition currentPos = dest;
    while (currentPos != myPos) {
        EMove currentMove = previous[currentPos];
        moves.pb(currentMove);
        currentPos = currentPos.Move(Inverse(currentMove));
    }
    reverse(all(moves));
    return true;
}

ShortestPathResult shortestPathFromOneToMany(const TBoard &board,  const TPosition &from,
        const TPositionsV& destinations, bool shortest, bool ignoreStones) {
    ShortestPathResult shortestPathResult;
    shortestPathResult.found = 0;
    for (int i = 0; i < destinations.size(); ++i) {
        vector<EMove> path;
        if (TakeShortestPath(board, from, destinations[i], path, shortest, ignoreStones)) {
            if (shortestPathResult.found) {
                if (shortestPathResult.path.size() > path.size()) {
                    shortestPathResult.path = path;
                }
            } else {
                shortestPathResult.found = true;
                shortestPathResult.path = path;
            }
        }
    }
    return shortestPathResult;
}

bool TakeShortestPath(const TBoard &board, const TPosition &dest, vector<EMove> &moves, bool shortest, bool ignoreStones)
{
    TPosition myPos = board.GetRobotPosition();
    return TakeShortestPath(board, myPos, dest, moves, shortest, ignoreStones);
}


void Bfs(const TBoard &board, const TPosition &start, vector< vector<int> > *distance, const TPosition *dest = NULL, bool ignoreStones = false) {
    vector< vector<int> > &d(*distance);
    d.assign(board.YMax, vector<int>(board.XMax, INF));

    d[start.Y][start.X] = 0;
    queue<TPosition> q;
    q.push(start);
    while (!q.empty() && (!dest || d[dest->Y][dest->X] >= INF)) {
        TPosition curPos = q.front();
        q.pop();
        
        for (int dir = Up; dir != Wait; ++dir) {
            TPosition newPos = curPos.Move(EMove(dir));
            if (board.IsValidPosition(newPos)) {
                if (d[newPos.Y][newPos.X] == INF) {
                    d[newPos.Y][newPos.X] = d[curPos.Y][curPos.X] + 1;
                    if (board.IsPassable(newPos) || (ignoreStones && board.GetCellType(newPos) == Rock))
                        q.push(newPos);
                }
            }
        }    
    } 
}

bool TakeBFS(const TBoard &board, const TPosition &dest, vector<vector<int> > *distance, vector<EMove> &moves, bool ignoreStones) {
    //vector< vector<int> > distance;
    vector< vector<int> > &d(*distance);
    Bfs(board, board.GetRobotPosition(), distance, &dest, ignoreStones);
    moves.clear();
    TPosition cur = dest;
    if (d[cur.Y][cur.X] >= INF) {
        return false;
    }
    while (d[cur.Y][cur.X] > 0) {
        for (int dir = Up; dir != Wait; ++dir) {
            TPosition next = cur.Move(EMove(dir));
            if (board.IsValidPosition(next) && (board.IsPassable(next) || (ignoreStones && board.GetCellType(next) == Rock)) && d[next.Y][next.X] < d[cur.Y][cur.X]) {
                moves.push_back(Inverse(EMove(dir)));
                cur = next;
                break;
            }
        }
    }
    reverse(all(moves));
    return true;
}

void CalcWallsToRemove(const TBoard &board, const TPosition &dest, vector<TPosition>* walls) {
    walls->clear();
    vector< vector<int> > fromRobot, fromDest;
    Bfs(board, board.GetRobotPosition(), &fromRobot);
    Bfs(board, dest, &fromDest);
    for (int y = 0; y < board.YMax; ++y)
        for (int x = 0; x < board.XMax; ++x)
            if (board.GetCellType(TPosition(x, y)) == Rock && 
                fromRobot[y][x] < INF && fromDest[y][x] < INF)
            {
                walls->push_back(TPosition(x, y));
            }
}
