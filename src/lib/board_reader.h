#pragma once
#include "common.h"
#include "board.h"
#include "flood.h"

vector<string> Split(string& s);
TBoard ReadBoardFromFile(const string& filename);
TBoard ReadBoardFromStdin();
