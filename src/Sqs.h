#ifndef SQS_H
#define SQS_H

#include <unordered_map>

#include "State.h"
#include "Symbol.h"

/**
 * To be used as a key value in the Sqs map.
 * Not to be instantiated by users.
 */
class SqsKey {
    private:
        State *q;
        SymbolString *s;

    public:
        SqsKey(State *q, SymbolString *s);
        bool equals(SqsKey const& other) const;
        std::size_t hash() const;
        SqsKey * clone() const;
        std::string to_string() const;
        ~SqsKey();
};

/**
 * The hasher required for the map.
 */
struct SqsKeyHash {
    inline std::size_t operator()(SqsKey* const& k) const {
        return k->hash();
    }
};

/**
 * The equality checker required for the map.
 */
struct SqsKeyEq {
    inline bool operator()(SqsKey const *lhs, SqsKey const *rhs) const {
        return lhs->equals(*rhs);
    }
};

/**
 * A (State, SymbolString)-StateSet map.
 */
class Sqs {
    private:
        std::unordered_map<SqsKey *, StateSet *, SqsKeyHash, SqsKeyEq> map;
    public:
        /**
         * Returns the StateSet mapped to the specified State-Symbolstring combination.
         * If such a set does not exist, one is allocated and returned.
         */
        StateSet * get(State *q, SymbolString *s);

        /**
         * Empty the map.
         */
        void clear();

        std::string to_string() const;

        ~Sqs();
};

#endif /* SQS_H  */
