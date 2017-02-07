#ifndef STATE_H
#define STATE_H

#include "Set.h"

/**
 * Represents a state in a PDA, labeled with an integer id.
 */
class State {
    private:
        int id;

    public:
        friend bool operator==(State const& lhs, State const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, State const& self) {
            return strm << self.to_string();
        }

        State(int id);
        bool equals(State const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
};

/**
 * The hasher required for the Set.
 */
struct StateHash {
    inline std::size_t operator()(State const& s) const {
        return s.hash();
    }
};

/**
 * The equality checker required for the Set.
 */
struct StateEq {
    inline bool operator()(State const lhs, State const rhs) const {
        return lhs.equals(rhs);
    }
};

typedef Set<State, StateHash, StateEq> StateSet;

#endif /* STATE_H */
