#include "traverser.h"

void TPartsTraverser::ReevalParts(int depth) {
    for (int i = 0; i < PartsCount; ++i) {
        PartsEval[i] = 1.0 * LambdasCount[i];
    }
    for (int i = 0; i < depth; ++i) {
        vector<double> newEval(PartsCount);
        for (int j = 0; j < PartsCount; ++j) {
            for (size_t k = 0; k < PartsLinks[j].size(); ++k) {
                newEval[j] += PartsEval[PartsLinks[j][k]] / PartsLinks[j].size();
            }
        }
        PartsEval = newEval;
    }
}

void TPartsTraverser::RecountLambdas(int part) {
    LambdasCount[part] = 0;
    for (size_t i = 0; i < Parts[part].size(); ++i) {
        LambdasCount[part] += Board->IsLambda(Parts[part][i]);
    }
}
