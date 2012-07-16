#pragma once

enum ECellType {
    Robot,
    Rock,
    ClosedLift,
    OpenLift,
    Wall,
    Lambda,
    Empty,
    Earth,
    Teleport,
    Beard,
    Razor,
    HighRock
};

inline bool IsTeleport(char c) {
    return (('A' <= c && c <= 'I') ||
            ('1' <= c && c <= '9'));
}

inline bool EitherRockOrHighRock(ECellType type) {
    return (type == Rock || type == HighRock);
}

inline ECellType CellTypeFromChar(char c)
{
    if (IsTeleport(c)) return Teleport;
    switch (c) {
        case 'R': return Robot;
        case '*': return Rock;
        case 'L': return ClosedLift;
        case 'O': return OpenLift;
        case '#': return Wall;
        case '\\': return Lambda;
        case ' ': return Empty;
        case '.': return Earth;
        case 'W': return Beard;
        case '!': return Razor;
        case '@': return HighRock;
        default:
            return Wall;
            fprintf(stderr, "Unexpected char: %c (%d)\n", c, c);
//            assert (false);
    }
    return Wall;
}

inline char CharFromCellType(ECellType type) {
    return "R*LO#\\ .TW!@"[type];
}
