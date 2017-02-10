#ifndef PDATRANSITION_H
#define PDATRANSITION_H

#include "State.h"
#include "Symbol.h"
#include "Set.h"

enum ExtensionStatus {NO_EXTENSION, START_UP, CLOSE_DOWN};

/**
 * Represents a transition in a PDA.
 */
class PDATransition {
    private:
        State *q;
        SymbolString *sigma;
        char symbol;
        State *r;
        SymbolString *tau;
        enum ExtensionStatus extension;
    
    public:
        friend bool operator==(PDATransition const& lhs, PDATransition const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, PDATransition const& self) {
            return strm << self.to_string();
        }

        PDATransition(State *q, SymbolString *sigma, char symbol, State *r, SymbolString *tau, ExtensionStatus extension = NO_EXTENSION);
        bool equals(PDATransition const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        PDATransition * clone() const;
        ~PDATransition();
};

/**
 * The hasher required for the Set.
 */
struct PDATransitionHash {
    inline std::size_t operator()(PDATransition* const& t) const {
        return t->hash();
    }
};

/**
 * The equality checker required for the Set.
 */
struct PDATransitionEq {
    inline bool operator()(PDATransition const *lhs, PDATransition const *rhs) const {
        return lhs->equals(*rhs);
    }
};

typedef Set<PDATransition, PDATransitionHash, PDATransitionEq> PDATransitionSet;

#endif /* PDATRANSITION_H */
