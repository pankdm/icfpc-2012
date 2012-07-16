#pragma once

#include "common.h"

class TGrowth
{
public:
    TGrowth()
        : Cooldown(25)
        , CurrentTimer(0)
    { }

    int Cooldown;
    int CurrentTimer;

    bool NextTurn() {
        ++CurrentTimer;
        if (CurrentTimer == Cooldown) {
            CurrentTimer = 0;
            return true;
        }
        return false;
    }
    string ToString() const
    {
        stringstream stream;
        stream << TO_DEBUG_STRING(Cooldown) << endl;
        stream << TO_DEBUG_STRING(CurrentTimer) << endl;
        return stream.str();
    }


};
