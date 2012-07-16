#include "greedy_solution.h"
#include "pathfinder.h"

bool TGreedySolution::FindNextTarget(
	const TPosition& currentPosition,
	TPosition& answer)
{
	bool found = false;
	TPosition result(-1, -1);
	for (int y = 0; y < Board.YMax; ++y) {
		for (int x = 0; x < Board.XMax; ++x) {
			TPosition next = TPosition(x, y);
			ECellType type = Board.GetCellType(next);
            if (UnreachableTarget.find(next) != UnreachableTarget.end()) {
                continue;
            }

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

void TGreedySolution::Init() {
	Info.push_back("");
	MoveId = 0;
    bool stop = false;
    while (!stop) {
		TPosition robotPosition = Board.GetRobotPosition();
		if (Board.GetGameState() != Running) break;
		TPosition next;

        while (true) {
            bool found = FindNextTarget(robotPosition, next);
            Info[MoveId] = "next target is " + next.ToString() + "\n";

            if (!found) {
                Info[MoveId] = "didn't found next cell";
                stop = true;
                break;
            }
            vector<EMove> moves;
            if (TakeShortestPath(Board, next, moves)) {
                for (int i = 0; i < sz(moves); ++i) {
                    Board.MakeMove(moves[i]);
                    MoveBuffer.push_back(moves[i]);
                    Targets.push_back(next);
                    Info.push_back("");
                    ++MoveId;
                }
                break;
            } else {
                UnreachableTarget.insert(next);
            }
        }

	    if (Board.GetRobotPosition() != next) {
		    Info[MoveId] = "robot now is at " + Board.GetRobotPosition().ToString() + '\n' + "robot didn't reached goal";
	        break;
		}
	}

    Info.push_back("finished");
    Targets.push_back(Board.GetRobotPosition());
	MoveId = 0;
}

EMove TGreedySolution::NextMove() {
    if (MoveId < sz(MoveBuffer)) {
		return MoveBuffer[MoveId++];
	} else {
		return Abort;
	}
}

string TGreedySolution::GetInfo() const {
	return Info[MoveId];
}

TPosition TGreedySolution::GetTarget() const {
    return Targets[MoveId];
}
