#pragma once
	
#include "common.h"
#include "cell_type.h"
#include "position.h"
#include "flood.h"
#include "teleports.h"
#include "growth.h"

enum EState {
    Running,
    Winning,
    Losing,
    Aborted
};

inline string StringFromEState(EState state) {
    if (state == Running) return "Running";
    if (state == Winning) return "Winning";
    if (state == Losing) return "Losing";
    if (state == Aborted) return "Aborted";
}

class TBoard
{
public:
    TBoard()
    { }

    explicit TBoard(const vector<string>& input);

    void MakeMove(EMove direction);
    void MakeMoves(const vector<EMove>& directions);
    EState GetGameState() const;

    ECellType GetCellType(TPosition pos) const;
    TPosition GetRobotPosition() const;
    TPosition GetLiftPosition() const;

    vector<TPosition> GetLambdasPositions() const;
    vector<TPosition> GetSonesPositions() const;

    bool IsPassable(const TPosition& pos) const;
    bool IsLambda(const TPosition& pos) const;
    bool IsRock(const TPosition& pos) const;

	vector<string> ToStringsVector() const;
    string ToString() const;

    // readonly
    int XMax;
    int YMax;
    int LambdasCount;
    int NumRazors;

    TPositionsV getNeighbours(const TPosition& position) const;

    bool IsValidPosition(const TPosition& position) const;

    const TFlood& GetFlood() const;
    void SetFlood(const TFlood& newFlood);

    void SetTeleports(const TTeleports& teleports);
    const TTeleports &GetTeleports() const;

    void SetGrowth(const TGrowth& growth);

    bool DebugMode;

private:
    TFlood Flood;
    TTeleports Teleports;
    TGrowth Growth;
    bool needGrowthUpdate;

    set<TPosition> WasMoved;

    TPosition RobotPosition;
    TPosition LiftPosition;
    EState GameState;

    set<TPosition> LambdasPositions;
    set<TPosition> StonesPositions;

    void InitPositions();

    typedef vector<vector<ECellType> > TField;
    TField Field;

    ECellType GetCellType(int x, int y) const;
    bool IsRock(int x, int y) const;

    void UpdateBoard();
    void UpdateCell(int x, int y, TField& dst);

    bool CanMoveStoneTo(const TPosition& stonePosition, EMove direction) const;
    void SetCellType(TPosition pos, ECellType newType);

    void DoMakeMove(EMove direction);
    void MoveRobotTo(const TPosition& newPosition);
    void ClearBeards(int x, int y);
    void MoveStoneTo(const TPosition& stonePosition, EMove direction);
    void UpdateStoneTo(TField& dst, int y, int x, ECellType rockType);

    string DebugInfo;
};
