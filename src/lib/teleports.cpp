
#include "teleports.h"
#include "cell_type.h"

TTeleports ConvertFromInput(
    const vector<string>& input,
    const vector <pair <char, char> >& teleportsInput)
{
    map<char, vector<TPosition> > helper;


    for (int y = 0; y < sz(input); ++y) {
        for (int x= 0; x < sz(input[y]); ++x) {
            char c = input[y][x];
            if (IsTeleport(c)) {
                helper[c].push_back(TPosition(x, y));
            }
        }
    }

    TTeleports res;
    for (int i = 0; i < sz(teleportsInput); ++i) {
        const pair <char, char>& pt = teleportsInput[i];
        const vector<TPosition>& v_in = helper[pt.first];
        const vector<TPosition>& v_out = helper[pt.second];
        for (int j = 0; j < sz(v_in); ++j) {
            for (int k = 0; k < sz(v_out); ++k) {
                res.AddTeleport(v_in[j], v_out[k]);
            }
        }
    }
    return res;
}
