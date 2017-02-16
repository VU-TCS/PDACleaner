#ifndef STATEVISITMAP_H
#define STATEVISITMAP_H

#include <unordered_map>

#include "State.h"

/**
 * A State-int map used to count visits to each State as part of
 * the add_eps_transitions function in the PDACleaner.
 */
class StateVisitMap {
    private:
        std::unordered_map<State *, int, StateHash, StateEq> map;

    public:
        void put(State *q, int c);
        int get(State *q);
        bool has_key(State *q) const;
        ~StateVisitMap();
};


#endif /* STATEVISITMAP_H */
