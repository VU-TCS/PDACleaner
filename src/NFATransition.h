#ifndef NFATRANSITION_H
#define NFATRANSITION_H

#include "State.h"
#include "Symbol.h"
#include "Set.h"

/**
 * Represents a transition in a NFA.
 */
class NFATransition {
    private:
        State *q;
        Symbol *symbol;
        State *r;
    
    public:
        friend bool operator==(NFATransition const& lhs, NFATransition const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, NFATransition const& self) {
            return strm << self.to_string();
        }

        NFATransition(State *q, Symbol *symbol, State *r);

        State * get_q();
        Symbol * get_symbol();
        State * get_r();

        bool equals(NFATransition const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        NFATransition * clone() const;
        ~NFATransition();
};

/**
 * The hasher required for the Set.
 */
struct NFATransitionHash {
    inline std::size_t operator()(NFATransition* const& t) const {
        return t->hash();
    }
};

/**
 * The equality checker required for the Set.
 */
struct NFATransitionEq {
    inline bool operator()(NFATransition const *lhs, NFATransition const *rhs) const {
        return lhs->equals(*rhs);
    }
};

typedef Set<NFATransition, NFATransitionHash, NFATransitionEq> NFATransitionSet;

#endif /* NFATRANSITION_H */
