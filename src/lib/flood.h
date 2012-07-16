#pragma once

#include "position.h"

class TFlood
{
public:
    TFlood()
        : Water(0)
        , Flooding(0)
        , WaterProof(10)
        , WaterProofLeft(10)
        , CurrentTimer(0)
        , RobotDestroyed(false)
    { }

    // readonly
    int Water;
    int Flooding;
    int WaterProof;

    int WaterProofLeft;
    int CurrentTimer;

    bool RobotDestroyed;

    void NextTurn(const TPosition& robotPosition)
    {
        // if robot goes up as the water rises
        // it works as he re entered the the water
        if (robotPosition.Y > Water) {
            WaterProofLeft = WaterProof;
        }

        // update level
        ++CurrentTimer;
        if (CurrentTimer == Flooding) {
            CurrentTimer = 0;
            ++Water;
        }

        // update robot state
        if (robotPosition.Y <= Water) {
            --WaterProofLeft;
            if (WaterProofLeft < 0) {
                RobotDestroyed = true;
            }
        } else {
            WaterProofLeft = WaterProof;
        }
    }

    string ToString() const
    {
        stringstream stream;
        stream << TO_DEBUG_STRING(Water) << endl;
        stream << TO_DEBUG_STRING(Flooding) << endl;
        stream << TO_DEBUG_STRING(WaterProof) << endl;
        stream << TO_DEBUG_STRING(WaterProofLeft) << endl;
        stream << TO_DEBUG_STRING(CurrentTimer) << endl;
        return stream.str();
    }
};

inline ostream& operator<<(ostream& o, const TFlood& v) {
    return o << v.ToString();
}

