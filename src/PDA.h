#ifndef PDA_H
#define PDA_H

#include "State.h"
#include "Symbol.h"
#include "PDATransition.h"

/**
 * Represents a Non-deterministic Pushdown Automaton.
 */
class PDA {
    private:
        std::string name;
        StateSet Q;
        SymbolSet Gamma;
        PDATransitionSet Delta;
        SymbolString Z;
        State q_0;
        StateSet F;
    
    public:
        friend bool operator==(PDA const& lhs, PDA const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, PDA const& self) {
            return strm << self.to_string();
        }

        PDA(std::string name, StateSet Q, SymbolSet Gamma, PDATransitionSet Delta, SymbolString Z, State q_0, StateSet F);
        bool equals(PDA const& other) const;
        std::string to_string() const;
};

#endif /* PDA_H */
