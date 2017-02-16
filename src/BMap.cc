#include "BMap.h"

#include <sstream>

StateSet * BMap::get(State *q) {
    auto it = map.find(q);
    if (it != map.end()) {
        return it->second;
    }

    StateSet *states = new StateSet();
    map.insert({q->clone(), states});
    return states;
}

typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator BMap::begin() const {
    return map.begin();
}

typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator BMap::end() const {
    return map.end();
}

BMap::~BMap() {
    for (auto it = map.begin(); it != map.end(); it++) {
        delete it->first;
        delete it->second;
    }
}

std::string BMap::to_string() const {
    std::stringstream ss;

    for (auto it = map.begin(); it != map.end(); it++) {
        ss << "Key: { " << *it->first << " } , Value: " << it->second->to_string("", " ")  << std::endl;
    }

    return ss.str();
}

