#include "annealing.h"
#include "scores.h"
#include "sandbox.h"

const int MAX_ITS = 50;

void TPathAnnealing::Init() {
    //srand((unsigned int)time(0));
    vector< pair<int, TPosition> > path(Board.GetLambdasPositions().size());
    for (size_t i = 0; i < path.size(); ++i) {
        path[i] = make_pair((Board.GetRobotPosition() ^ Board.GetLambdasPositions()[i]), Board.GetLambdasPositions()[i]);
    }
    sort(all(path));
    CurrentPath.resize(path.size());
    for (size_t i = 0; i < path.size(); ++i) {
        CurrentPath[i] = path[i].second;
    }
    Lambdas = set<TPosition>(CurrentPath.begin(), CurrentPath.end());
    for (TTeleports::TTo::const_iterator it = Board.GetTeleports().To.begin(); it != Board.GetTeleports().To.end(); ++it) {
        //cerr << it->first.ToString() << '\n';
        Teleports.push_back(it->first);
    }
    //random_shuffle(CurrentPath.begin(), CurrentPath.end());
    CurrentPath.push_back(Board.GetLiftPosition());
    BestPath = CurrentPath;
    CurrentScore = BestScore = SandBox.TryTargetSequence(CurrentPath);
}

void TPathAnnealing::DoIterations(int itsToDo) {
    for (int it = 0; it < itsToDo; ++it) {
        if (STOP_ALL_ITERATIONS) break;
        ++Iters;
        if (Iters % 100 == 0) {
             //cerr << "Iteration " << Iters << ": " << CurrentScore << '\n';
        }
        NewPath = MakeRandomChange(CurrentPath);
        NewScore = SandBox.TryTargetSequence(NewPath);
        
        if (NewScore > CurrentScore || Lucky()) {
            CurrentScore = NewScore;
            CurrentPath = NewPath;
            if (CurrentScore > BestScore) {
                BestScore = CurrentScore;
                BestPath = CurrentPath;
                //cerr << BestScore << "!\n";
            }
        }
    }
}

int TPathAnnealing::GetBestScore() const {
    return BestScore;
}

vector<TPosition> TPathAnnealing::GetBestPath() const {
    return BestPath;
}

vector<EMove> TPathAnnealing::GetBestMoves() {
    vector<EMove> moves;
    SandBox.TryTargetSequence(BestPath, moves);
    return moves;
}

bool TPathAnnealing::Lucky() const {
    return uniform() < pow(1.0 * Iters, -0.2) * pow(1.0 * (max(CurrentScore, 1)) / (max(NewScore, 1)), -7.0);
}

vector<TPosition> TPathAnnealing::MakeRandomChange(const vector<TPosition> &path) {
    double p = uniform();
    double p1 = 0.6, p2 = 0.05, p3 = 0.2, p4 = 0.05;
    if (p < p1) {
        return RandomTranspose(path);
    } else {
        p -= p1;
    }
    if (p < p2) {
        return ShuffleAll(path);
    } else {
        p -= p2;
    }
    if (p < p3) {
        return DeleteRandomWaypoint(path);
    } else {
        p -= p3;
    }
    if (p < p4) {
        return AddRandomTeleport(path);
    } else {
        p -= p4;
    }
    return AddRandomWaypoint(path);
}

vector<TPosition> TPathAnnealing::AddRandomWaypoint(const vector<TPosition>& path_) {
    vector<TPosition> path(path_);
    int in = randMod(path.size());
    TPosition pos(randMod(Board.XMax), randMod(Board.YMax));
    int its = 0;
    while (its < MAX_ITS && (!Board.IsPassable(pos) || Board.GetCellType(pos) == Lambda)) {
        pos = TPosition(randMod(Board.XMax), randMod(Board.YMax));
        ++its;
    }
    if (its == MAX_ITS) {
        return path;
    }
    path.insert(path.begin() + in, pos);
    return path;
}

vector<TPosition> TPathAnnealing::AddRandomTeleport(const vector<TPosition> &path_) {
    vector<TPosition> path = path_;
    if (Teleports.empty()) {
        return path;
    }
    size_t in = randMod(path.size());
    TPosition pos = Teleports[randMod(Teleports.size())];
    path.insert(path.begin() + in, pos);
    return path;
}

/*vector<TPosition> TPathAnnealing::AddRandomStone(const vector<TPosition>& path_) {
    vector<TPosition> path(path_);
    vector<TPosition> stones = Board.GetSonesPositions();
    if (stones.empty()) {
        return path;
    }
    size_t in = randMod(path.size());
    TPosition pos = stones[randMod(stones.size())];
    path.insert(path.begin() + in, pos);
    return path;
}*/

vector<TPosition> TPathAnnealing::DeleteRandomWaypoint(const vector<TPosition>& path_) {
    vector<TPosition> path(path_);
    size_t in = randMod(path.size() - 1);
    int its = 0;
    while (its < MAX_ITS && Lambdas.find(path[in]) != Lambdas.end()) {
        in = randMod(path.size() - 1);
        ++its;
    }
    if (its == MAX_ITS) {
        return path;
    }
    path.erase(path.begin() + in);
    return path;
}

vector<TPosition> TPathAnnealing::RandomTranspose(const vector<TPosition>& path) {
    if (path.size() < 3) {
        return path;
    }
    int i1 = randMod(path.size() - 2);
    int i2 = randMod(path.size() - 2);
    vector<TPosition> newPath(path);
    newPath[i1] = path[i2];
    newPath[i2] = path[i1];
    //size_t i2 = randMod(path.size() - 2);
//    swap(path[i1], path[i1 + 1]);
    return newPath;
}

vector<TPosition> TPathAnnealing::ShuffleAll(const vector<TPosition>& path_) {
    vector<TPosition> path(path_);
    if (path.size() == 1) {
        return path;
    }
    size_t i1 = randMod(path.size() - 1);
    size_t i2 = randMod(path.size() - 1);
    if (i1 > i2) {
        swap(i1, i2);
    }
    random_shuffle(path.begin() + i1, path.begin() + i2);
    return path;
}

