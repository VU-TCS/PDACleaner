#ifndef BMAP_H
#define BMAP_H

#include <unordered_map>

#include "State.h"

class BMap {
    private:
        std::unordered_map<State *, StateSet *, StateHash, StateEq> map;

    public:
        StateSet * get(State *q);
        typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator begin() const;
        typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator end() const;
        ~BMap();
};


#endif /* BMAP_H */
