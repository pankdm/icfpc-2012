#include "board.h"

TPositionsV TBoard::getNeighbours(const TPosition& position) const {
    TPositionsV res;
    if (IsValidPosition(position.Move(Left)))
        res.push_back(position.Move(Left));
    if (IsValidPosition(position.Move(Right)))
        res.push_back(position.Move(Right));
    if (IsValidPosition(position.Move(Up)))
        res.push_back(position.Move(Up));
    if (IsValidPosition(position.Move(Down)))
        res.push_back(position.Move(Down));
    return res;
}

bool TBoard::IsValidPosition(const TPosition &position) const
{
    return position.X >= 0 &&
           position.Y >= 0 &&
           position.X < XMax &&
           position.Y < YMax;
}

const TFlood& TBoard::GetFlood() const
{
    return Flood;
}

void TBoard::SetFlood(const TFlood &newFlood)
{
    Flood = newFlood;
}

void TBoard::SetTeleports(const TTeleports &teleports)
{
    Teleports = teleports;
    TTeleports::TTo& to = Teleports.To;

    for (TTeleports::TTo::iterator it = to.begin(); it != to.end(); ++it) {
        SetCellType(it->first, Teleport);
        SetCellType(it->second, Wall);
    }
}

const TTeleports& TBoard::GetTeleports() const
{
    return Teleports;
}

void TBoard::SetGrowth(const TGrowth& growth)
{
    Growth = growth;
}

TBoard::TBoard(const vector<std::string>& input)
    : GameState(Running)
    , LambdasCount(0)
    , XMax(0)
    , YMax(0)
    , DebugMode(false)
    , NumRazors(0)
{
	YMax = sz(input);

    for (int y = 0; y < YMax; ++y) {
        XMax = max(XMax, sz(input[y]));
    }

//	cerr << XMax << ' ' << YMax << '\n';

    Field.assign(YMax, vector<ECellType>(XMax, Empty));
    for (int y = 0; y < YMax; ++y) {
        for (int x = 0; x < sz(input[y]); ++x) {
            ECellType type = CellTypeFromChar(input[y][x]);
            if (type == Robot) {
                RobotPosition = TPosition(x, y);
                type = Empty;
            }
            if (type == Lambda) {
                ++LambdasCount;
            }
            if (type == HighRock) {
                ++LambdasCount;
            }

            Field[y][x] = type;

        }
    }

    InitPositions();
}

void TBoard::MakeMove(EMove direction)
{
    if (DebugMode) {
        DebugInfo.clear();
    }
    if (GameState == Running) {
        DoMakeMove(direction);
        UpdateBoard();
    }
}

void TBoard::MakeMoves(const vector<EMove>& directions)
{
    for (int i = 0; i < sz(directions); ++i) {
        MakeMove(directions[i]);
    }
}

EState TBoard::GetGameState() const
{
    return GameState;
}

void TBoard::DoMakeMove(EMove direction)
{
    if (direction == Wait) {
        return;
    }
    if (direction == Shave) {
        if (NumRazors > 0) {
            --NumRazors;
            ClearBeards(RobotPosition.X, RobotPosition.Y);
        }
    }

    if (direction == Abort) {
        GameState = Aborted;
        return;
    }

    TPosition nextPosition = RobotPosition.Move(direction);
    if (IsPassable(nextPosition)) {
        MoveRobotTo(nextPosition);
    }

    if (IsRock(nextPosition) &&
        CanMoveStoneTo(nextPosition, direction))
    {
        MoveStoneTo(nextPosition, direction);
        MoveRobotTo(nextPosition);
    }
}

bool TBoard::IsPassable(const TPosition &pos) const
{
    if (!IsValidPosition(pos)) {
        return false;
    }
    ECellType type = GetCellType(pos);
    if (type == Earth || type == Empty || type == Lambda || type == OpenLift) return true;
    if (type == Robot) return true; //Consider Robot to be Empty
    if (type == Teleport) return true; // Teleports are passable
    if (type == Razor) return true;// Razors are passable
    return false;
}

bool TBoard::IsLambda(const TPosition &pos) const {
    return GetCellType(pos) == Lambda;
}

bool TBoard::IsRock(const TPosition &pos) const
{
    return IsRock(pos.X, pos.Y);
}

ECellType TBoard::GetCellType(int x, int y) const
{
    if (x == RobotPosition.X && y == RobotPosition.Y) {
        return Robot;
    }
    return Field[y][x];
}

bool TBoard::IsRock(int x, int y) const
{
    return (GetCellType(x, y) == Rock ||
            GetCellType(x, y) == HighRock);
}

ECellType TBoard::GetCellType(TPosition pos) const
{
   return GetCellType(pos.X, pos.Y);
}

TPosition TBoard::GetRobotPosition() const
{
    return RobotPosition;
}

TPosition TBoard::GetLiftPosition() const
{
    return LiftPosition;
}

vector<TPosition> TBoard::GetLambdasPositions() const
{
    return vector<TPosition>(all(LambdasPositions));
}

vector<TPosition> TBoard::GetSonesPositions() const
{
    return vector<TPosition>(all(StonesPositions));
}

void TBoard::SetCellType(TPosition pos, ECellType newType)
{
    if (newType == Robot) newType = Empty;
    Field[pos.Y][pos.X] = newType;
}

void TBoard::MoveRobotTo(const TPosition &newPosition)
{
    if (GetCellType(newPosition) == Lambda) {
        --LambdasCount;
    }
    if (GetCellType(newPosition) == Razor) {
        ++NumRazors;
    }

    if (GetCellType(newPosition) == OpenLift) {
        GameState = Winning;
    }
    if (GetCellType(newPosition) == Teleport) {

        TPosition to = Teleports.To[newPosition];

        // remove all entrances
        const vector<TPosition>& in = Teleports.From[to];
        for (int i = 0; i < sz(in); ++i) {
            if (DebugMode) {
                DebugInfo += "Teleport at " + in[i].ToString() + " was removed\n";
            }
            SetCellType(in[i], Empty);
        }

        SetCellType(RobotPosition, Empty);
        SetCellType(to, Empty);
        RobotPosition = to;

        if (DebugMode) {
            DebugInfo += "Entrance " + to.ToString() + " became visible\n";
            DebugInfo += "Robot was teleported to " + to.ToString() + "\n";
        }

    } else {
        SetCellType(RobotPosition, Empty);
        SetCellType(newPosition, Robot);
        RobotPosition = newPosition;
    }
}

void TBoard::ClearBeards(int x, int y)
{
    for (int xx = x - 1; xx <= x + 1; ++xx) {
        for (int yy = y - 1; yy <= y + 1; ++yy) {
            if (GetCellType(xx, yy) == Beard) {
                SetCellType(TPosition(xx, yy), Empty);
            }
        }
    }
}

void TBoard::MoveStoneTo(const TPosition &stonePosition, EMove direction)
{
    TPosition nextPosition = stonePosition.Move(direction);
    SetCellType(nextPosition, GetCellType(stonePosition));
    SetCellType(stonePosition, Empty);
}

bool TBoard::CanMoveStoneTo(const TPosition &stonePosition, EMove direction) const
{
    if (direction == Up) return false;
    if (direction == Down) return false;
    TPosition nextPosition = stonePosition.Move(direction);
    if (GetCellType(nextPosition) == Empty) return true;
    return false;
}

void TBoard::UpdateBoard()
{
    TField NewField = Field;

    needGrowthUpdate = Growth.NextTurn();

    WasMoved.clear();
    for (int x = 0; x < XMax; ++x) {
        for (int y = 0; y < YMax; ++y) {
            UpdateCell(x, y, NewField);
        }
    }

    TPosition up = RobotPosition.Move(Up);
    if (EitherRockOrHighRock(NewField[up.Y][up.X])
        && NewField[up.Y][up.X] != Field[up.Y][up.X])
    {
        if (DebugMode) {
            DebugInfo += "Robot was destroyed by stone\n";
        }
        GameState = Losing;
    }

    // update flood
    Flood.NextTurn(RobotPosition);
    if (Flood.RobotDestroyed) {
        if (DebugMode) {
            DebugInfo += "Robot got flooded\n";
        }
        GameState = Losing;
    }

    if (needGrowthUpdate && DebugMode) {
        DebugInfo += "Growth will be updated\n";
    }

    Field = NewField;
    InitPositions();
}

void TBoard::UpdateCell(int x, int y, TBoard::TField& dst)
{
    ECellType type = GetCellType(x, y);
    if (IsRock(x, y) &&
        GetCellType(x, y - 1) == Empty)
    {
        dst[y][x] = Empty;
        UpdateStoneTo(dst, y - 1, x, type);
    }

    if (IsRock(x, y) &&
        IsRock(x, y - 1))
    {
        if (GetCellType(x + 1, y) == Empty &&
            GetCellType(x + 1, y - 1) == Empty)
        {
            // move right
            dst[y][x] = Empty;
            UpdateStoneTo(dst, y - 1, x + 1, type);
        } else {
            if (GetCellType(x - 1, y) == Empty &&
                GetCellType(x - 1, y - 1) == Empty)
            {
                // move left
                dst[y][x] = Empty;
                UpdateStoneTo(dst, y - 1, x - 1, type);
            }
        }
    }

    if (IsRock(x, y) &&
        GetCellType(x, y - 1) == Lambda &&
        GetCellType(x + 1, y) == Empty &&
        GetCellType(x + 1, y - 1) == Empty)
    {
        dst[y][x] = Empty;
        UpdateStoneTo(dst, y - 1, x + 1, type);
    }

    if (GetCellType(x, y) == ClosedLift && LambdasCount == 0) {
        dst[y][x] = OpenLift;
    }

    if (needGrowthUpdate) {
        if (GetCellType(x, y) == Beard) {
            for (int xx = x - 1; xx <= x + 1; ++xx) {
                for (int yy = y - 1; yy <= y + 1; ++yy) {
                    TPosition pos(xx, yy);
                    if (IsValidPosition(pos) &&
                        GetCellType(xx, yy) == Empty) {
                        dst[yy][xx] = Beard;
                    }
                }
            }
        }
    }
}

vector<string> TBoard::ToStringsVector() const {
	vector<string> res(YMax, string (XMax, '?'));
    for (int y = YMax - 1; y >= 0; --y) {
        for (int x = 0; x < sz(Field[y]); ++x) {
			res[y][x] = CharFromCellType(GetCellType(x, y));
		}
	}
	return res;
}

string TBoard::ToString() const {
    vector<string> output = ToStringsVector();

    string telepInfo = Teleports.ShowOnMap(output);
    SetCellByPosition(RobotPosition, CharFromCellType(Robot), output);

    stringstream stream;
    for (int y = YMax - 1 ; y >= 0; --y) {
        string shownFlood = " ";
        if (y > Flood.Water) {
            shownFlood += " ";
        } else {
            shownFlood += "~";
        }
        stream << output[y] << shownFlood << endl;
    }

    stream << "Lambdas count: " << LambdasCount << endl;
    stream << "Robot position: " << RobotPosition << endl;
    stream << StringFromEState(GameState) << endl;

    stream << endl;
    stream << Flood.ToString() << endl;
    stream << Growth.ToString();
    stream << TO_DEBUG_STRING(NumRazors) << endl;

    stream << telepInfo << endl;

    stream << DebugInfo << endl;

    return stream.str();
}

void TBoard::UpdateStoneTo(TField& dst, int y, int x, ECellType rockType)
{
    dst[y][x] = rockType;
    if (rockType == HighRock) {
        if (dst[y-1][x] != Empty) {
            dst[y][x] = Lambda;
            if (DebugMode) {
                DebugInfo += "At " + TPosition(x, y).ToString() + " was crashed to lambda\n";
            }
        }
    }
}

void TBoard::InitPositions()
{
    LambdasPositions.clear();
    StonesPositions.clear();
    for (int y = 0; y < YMax; ++y) {
        for (int x = 0; x < sz(Field[y]); ++x) {
            TPosition now(x, y);
            ECellType type = GetCellType(now);
            if (type == Lambda) {
                LambdasPositions.insert(now);
            }
            if (type == Rock) {
                StonesPositions.insert(now);
            }
            if (type == OpenLift || type == ClosedLift) {
                LiftPosition = now;
            }
        }
    }
}
