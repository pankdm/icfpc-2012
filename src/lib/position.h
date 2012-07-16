#pragma once

#include "common.h"
#include "move.h"

struct TPosition
{
    int X;
    int Y;

    TPosition(int x, int y)
        : X(x)
        , Y(y)
    { }

    ~TPosition()
    { }

    TPosition()
        : X(0)
        , Y(0)
    { }

    TPosition operator+(const TPosition &pos) const {
        return TPosition(X + pos.X, Y + pos.Y);
    }

    int operator^(const TPosition &pos) const {
        return abs(X - pos.X) + abs(Y - pos.Y);
    }

    bool operator<(const TPosition &pos) const {
        if (X != pos.X) {
            return X < pos.X;
        }
        return Y < pos.Y;
    }

	bool operator==(const TPosition &pos) const {
		return X == pos.X && Y == pos.Y;
	}

	bool operator!=(const TPosition &pos) const {
		return !(*this == pos);
	}

    string ToString() const {
        stringstream stream;
        stream << "(" << X << ", " << Y << ") ";
        return stream.str();
    }

    TPosition Move(EMove direction) const {
        TPosition newPos = *this;
        switch (direction) {
            case Up: ++newPos.Y; break;
            case Down: --newPos.Y; break;
            case Left: --newPos.X; break;
            case Right: ++newPos.X; break;
        }
        return newPos;
    }
	
};

typedef vector<TPosition> TPositionsV;

inline ostream& operator<<(ostream& o, const TPosition& v) {
    return o << v.ToString();
}

inline void SetCellByPosition(const TPosition& pos, char c,  vector<string>& output) {
    output[pos.Y][pos.X] = c;
}
