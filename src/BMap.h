#ifndef BMAP_H
#define BMAP_H

#include <unordered_map>

#include "State.h"

/**
 * A State-StateSet map used to efficiently fill the Sqs map used in the
 * cleaning algorithm.
 */
class BMap {
    private:
        std::unordered_map<State *, StateSet *, StateHash, StateEq> map;

    public:
        /**
         * Returns the StateSet mapped to the specified State.
         * If such a set does not exist, one is allocated and returned.
         */
        StateSet * get(State *q);

        typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator begin() const;
        typename std::unordered_map<State *, StateSet *, StateHash, StateEq>::const_iterator end() const;
        std::string to_string() const;
        ~BMap();
};


#endif /* BMAP_H */
