#ifndef SQS_H
#define SQS_H

#include <unordered_map>

#include "State.h"
#include "Symbol.h"

class SqsKey {
    private:
        State *q;
        SymbolString *s;

    public:
        SqsKey(State *q, SymbolString *s);
        bool equals(SqsKey const& other) const;
        std::size_t hash() const;
        SqsKey * clone() const;
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

class Sqs {
    private:
        std::unordered_map<SqsKey *, StateSet *, SqsKeyHash, SqsKeyEq> map;
    public:
        StateSet * get(State *q, SymbolString *s);
        void clear();
        ~Sqs();
};

#endif /* SQS_H  */
