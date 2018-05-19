#include "correspondence.h"

correspondence::correspondence() {}
correspondence::~correspondence() {}

void correspondence::add_corr(int id0, int id1) {
    // corr.insert(id0, id1);
    QPair<int, int> pair;
    pair.first = id0;
    pair.second = id1;
    corr.push_back(pair);
}
