#pragma once

#include "position.h"

class TTeleports
{
public:

    typedef map<TPosition, TPosition> TTo;
    typedef map<TPosition, vector<TPosition> > TFrom;

    //readonly
    TTo To;
    TFrom From;

    void AddTeleport(const TPosition& from, const TPosition& to) {
        To[from] = to;
        From[to].push_back(from);
    }

    string ShowOnMap(vector<string>& output) const
    {
        char t_in = 'A';
        char t_out = '1';
        stringstream debugInfo;
        for (TFrom::const_iterator it = From.begin(); it != From.end(); ++it) {
            const vector<TPosition> vp = it->second;
            SetCellByPosition(it->first, t_out, output);
//            for (int i = 0; i < sz(vp); ++i) {
//                SetCellByPosition(vp[i], t_in, output);
//            }
            debugInfo << t_in << " --> " << t_out << endl;
            ++t_in;
            ++t_out;
        }
        return debugInfo.str();
    }
};

TTeleports ConvertFromInput(
    const vector<string>& input,
    const vector <pair <char, char> >& teleportsInput);
