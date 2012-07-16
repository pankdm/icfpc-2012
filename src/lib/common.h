#pragma once

#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <algorithm>
#include <bitset>
#include <set>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <map>
#include <list>
#include <iostream>
#include <queue>
#include <assert.h>
#include <ctime>

using namespace std;

#define sz(x) ((int)x.size())
#define all(x) (x).begin(), (x).end()
#define pb(x) push_back(x)
#define mp(x, y) make_pair(x, y)

//#define UNREACHABLE() assert(false);

template<class T>
inline ostream& operator<<(ostream& o, const vector<T>& v) {
    o << "[";
    for (int i = 0; i < v.size(); ++i) {
        o << v[i];
        if (i + 1 < v.size())
            o << ", ";
    }
    o << "]";
    return o;
}

inline void REQUARE(bool st, const string& s) {
    if (!st) {
        std::cerr << s << std::endl;
        throw exception();
    }
}

inline void UNREACHABLE(const string& s = "") {
	std::cerr << s << std::endl;
	assert(false);
}

#define INFO(x) cerr << x << endl;
#define debug(x) cerr << #x << ": " << x << endl;
#define TO_DEBUG_STRING(x) #x << ": " << x << " "

const int INF = static_cast<int>(1e9);

inline int BitCount(int n) {
    int r=0;
    while(n) {
        r += (n & 1);
        n >>= 1;
    }
    return r;
}

inline int ToInt(const string& s) {
    stringstream o;
    o << s;
    int n;
    o >> n;
    return n;
}

inline double uniform() {
    double res = 0.0;
    for (int i = 0; i < 2; ++i) {
        res = (res + rand()) / RAND_MAX;
    }
    return res;
}

inline int randMod(int N) {
    if (N <= 0) {
        return 0;
    }
    int res = 0;
    for (int i = 0; i < 2; ++i) {
        res = (res * RAND_MAX + rand()) % N;
    }
    if (res < 0) res += N;
    return res;
}

template<class T>
void Append(vector<T>& to, const vector<T>& added)
{
    for (int i = 0; i < sz(added); ++i) {
        to.push_back(added[i]);
    }
}
