#include "Sqs.h"

/*** SqsKey  ***/

SqsKey::SqsKey(State *q, SymbolString *s) {
    this->q = q->clone();
    this->s = s->clone();
}

bool SqsKey::equals(SqsKey const& other) const {
    return q->equals(*other.q) && s->equals(*other.s);
}

std::size_t SqsKey::hash() const {
    std::size_t res = 17;
    res = res * 19 * q->hash();
    res = res * 23 * s->hash();
    return res;
}

SqsKey * SqsKey::clone() const {
    return new SqsKey(q, s);
}

SqsKey::~SqsKey() {
    delete q;
    delete s;
}

/*** Sqs ***/

StateSet * Sqs::get(State *q, SymbolString *s) {
    SqsKey key(q, s);

    auto it = map.find(&key);
    if (it != map.end()) {
        return it->second;
    }

    StateSet *states = new StateSet();
    map.insert({key.clone(), states});
    return states;
}

void Sqs::clear() {
    for (auto it = map.begin(); it != map.end(); it++) {
        delete it->first;
        delete it->second;
    }
    
    map.clear();
}

Sqs::~Sqs() {
    for (auto it = map.begin(); it != map.end(); it++) {
        delete it->first;
        delete it->second;
    }
}
