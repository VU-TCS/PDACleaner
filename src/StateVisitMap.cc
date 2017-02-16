#include "StateVisitMap.h"

void StateVisitMap::put(State *q, int c) {
    map.insert({q->clone(), c});
}

int StateVisitMap::get(State *q) {
    auto it = map.find(q);
    if (it != map.end()) {
        return it->second;
    }
    return 0;
}

bool StateVisitMap::has_key(State *q) const {
    auto it = map.find(q);
    return it != map.end();
}

StateVisitMap::~StateVisitMap() {
    for (auto it = map.begin(); it != map.end(); it++) {
        delete it->first;
    }
}
