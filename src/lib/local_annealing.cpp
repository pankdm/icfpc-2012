#include "local_annealing.h"
#include "scores.h"
#include "sandbox.h"

void TLocalPathAnnealing::Init() {
    srand((unsigned int)time(0));
    for (size_t i = 0; i < Part.size(); ++i) {
        if (Board.IsLambda(Part[i])) {
            CurrentPath.push_back(Part[i]);
        }
    }
    if (Out.size() == 0)
        return;
    Lambdas = set<TPosition>(CurrentPath.begin(), CurrentPath.end());
    random_shuffle(CurrentPath.begin(), CurrentPath.end());
    CurrentPath.push_back(Out[randMod(Out.size())]);
    BestPath = CurrentPath;
    CurrentScore = BestScore = SandBox.TryTargetSequence(CurrentPath);
}

void TLocalPathAnnealing::DoIterations(int itsToDo) {
    if (Out.size() == 0)
        return;
    for (int it = 0; it < itsToDo; ++it) {
        ++Iters;
        if (Iters % 100 == 0) {
             cerr << "Iteration " << Iters << ": " << CurrentScore << '\n';
        }
        NewPath = MakeRandomChange(CurrentPath);
        NewScore = SandBox.TryTargetSequence(NewPath);
        
        if (NewScore > CurrentScore || Lucky()) {
            CurrentScore = NewScore;
            CurrentPath = NewPath;
            if (CurrentScore > BestScore) {
                BestScore = CurrentScore;
                BestPath = CurrentPath;
                cerr << BestScore << "!\n";
            }
        }
    }
}

int TLocalPathAnnealing::GetBestScore() const {
    return BestScore;
}

vector<TPosition> TLocalPathAnnealing::GetBestPath() const {
    return BestPath;
}

vector<EMove> TLocalPathAnnealing::GetBestMoves() {
    vector<EMove> moves;
    SandBox.TryTargetSequence(BestPath, moves);
    return moves;
}

bool TLocalPathAnnealing::Lucky() const {
    return uniform() < pow(0.95, 0.01 * Iters) + pow(0.7, 1.0 * (max(BestScore, 1)) / (max(NewScore, 1)));
}

vector<TPosition> TLocalPathAnnealing::MakeRandomChange(const vector<TPosition> &path) {
    double p = uniform();
    double p1 = 0.6, p2 = 0.1, p3 = 0.1, p4 = 0.1;
    if (p < p1) {
        return RandomTranspose(path);
    } else {
        p -= p1;
    }
    if (p < p2) {
        return AddRandomWaypoint(path);
    } else {
        p -= p2;
    }
    if (p < p3) {
        return DeleteRandomWaypoint(path);
    } else {
        p -= p3;
    }
    if (p < p4) {
        return ChangeFinish(path);
    } else {
        p -= p4;
    }
    return ShuffleAll(path);//AddRandomWaypoint(path);
}

vector<TPosition> TLocalPathAnnealing::AddRandomWaypoint(vector<TPosition> path) {
    size_t in = randMod(path.size());
    TPosition pos = Part[randMod(Part.size())];
    int its = 0;
    while (its < 100 && (!Board.IsPassable(pos) || Board.GetCellType(pos) == Lambda)) {
        pos = Part[randMod(Part.size())];
        ++its;
    }
    path.insert(path.begin() + in, pos);
    return path;
}

/*vector<TPosition> TLocalPathAnnealing::AddRandomStone(vector<TPosition> path) {
    vector<TPosition> stones = Board.GetSonesPositions();
    if (stones.empty()) {
        return path;;
    }
    size_t in = randMod(path.size());
    TPosition pos = stones[randMod(stones.size())];
    path.insert(path.begin() + in, pos);
    return path;
}*/

vector<TPosition> TLocalPathAnnealing::DeleteRandomWaypoint(vector<TPosition> path) {
    if (path.size() == 1) {
        return path;
    }
    size_t in = randMod(path.size() - 1);
    int its = 0;
    while (its < 100 && Lambdas.find(path[in]) != Lambdas.end()) {
        in = randMod(path.size() - 1);
        ++its;
    }
    if (Lambdas.find(path[in]) != Lambdas.end()) {
        return path;
    }
    path.erase(path.begin() + in);
    return path;
}

vector<TPosition> TLocalPathAnnealing::RandomTranspose(vector<TPosition> path) {
    if (path.size() < 3) {
        return path;
    }
    size_t i1 = randMod(path.size() - 2);
    size_t i2 = randMod(path.size() - 2);
    swap(path[i1], path[i2]);
    return path;
}

vector<TPosition> TLocalPathAnnealing::ShuffleAll(vector<TPosition> path) {
    if (path.size() == 1) {
        return path;
    }
    random_shuffle(path.begin(), path.end() - 1);
    return path;
}

vector<TPosition> TLocalPathAnnealing::ChangeFinish(vector<TPosition> path) {
    path[path.size() - 1] = Out[randMod(Out.size())];
    return path;
}
