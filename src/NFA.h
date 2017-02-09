#ifndef NFA_H
#define NFA_H

#include "State.h"
#include "Symbol.h"
#include "NFATransition.h"

/**
 * Represents a Non-deterministic Finite Automaton.
 */
class NFA {
    private:
        StateSet *Q;
        SymbolSet *Sigma;
        NFATransitionSet *Delta;
        State *q_0;
        StateSet *F;
    
    public:
        friend bool operator==(NFA const& lhs, NFA const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, NFA const& self) {
            return strm << self.to_string();
        }

        NFA(StateSet *Q, SymbolSet *Sigma, NFATransitionSet *Delta, State *q_0, StateSet *F);

        StateSet * get_Q();
        SymbolSet * get_Sigma();
        NFATransitionSet * get_Delta();
        State * get_q_0();
        StateSet * get_F();

        bool equals(NFA const& other) const;
        std::string to_string() const;
        NFA * clone() const;
        ~NFA();
};

#endif /* NFA_H */
