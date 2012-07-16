#include "board_reader.h"
#include "teleports.h"

vector<string> Split(string& s) {
    vector<string> res;
    int index = 0;
    while (true) {
        int now = index;
        index = s.find(' ', now);
        string t;
        if (index == string::npos) {
            t = s.substr(now, index);
        } else {
            t = s.substr(now, index - now);
        }
        if (!t.empty()) {
            res.push_back(t);
        }
        if (index == string::npos) {
            break;
        }
        ++index;
    }
    return res;
}

TBoard ReadBoardFromStream(istream& stream) {
    string s;
    vector < string > input;
    bool foundMetadata = false;

    TFlood flood;
    TGrowth growth;
    int numRazors = 0;
    vector<pair<char, char> > teleportsInput;
    while (getline(stream, s)) {
        //cout << s << endl;

        if (s.empty()) {
            foundMetadata = true;
            continue;
        }

        if (!foundMetadata) {
            input.push_back(s);
        } else {
            vector<string> parts = Split(s);
            if (sz(parts) > 0) {
                string first = parts[0];

                if (first == "Water") {
                    flood.Water = ToInt(parts[1]);
                }
                if (first == "Flooding") {
                    flood.Flooding = ToInt(parts[1]);
                }
                if (first == "Waterproof") {
                    flood.WaterProof = ToInt(parts[1]);
                }
                if (first == "Trampoline") {
                    assert (sz(parts[1]) == 1);
                    assert (sz(parts[3]) == 1);
                    char t_in = parts[1][0];
                    char t_out = parts[3][0];
                    teleportsInput.push_back(mp(t_in, t_out));
                }
                if (first == "Growth") {
                    growth.Cooldown = ToInt(parts[1]);
                }
                if (first == "Razors") {
                    numRazors = ToInt(parts[1]);
                }
            }
        }
    }

    reverse(all(input));

    TBoard board(input);

    board.SetFlood(flood);
    board.SetTeleports(ConvertFromInput(input, teleportsInput));
    board.SetGrowth(growth);
    board.NumRazors = numRazors;
    return board;
}


TBoard ReadBoardFromFile(const string& filename) {
    ifstream stream(filename.c_str(), ifstream::in);
    return ReadBoardFromStream(stream);
}

TBoard ReadBoardFromStdin() {
    return ReadBoardFromStream(cin);
}

