#include "Sqs.h"

#include <sstream>

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

std::string SqsKey::to_string() const {
    return q->to_string() + " , " + s->to_string();
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

std::string Sqs::to_string() const {
    std::stringstream ss;

    for (auto it = map.begin(); it != map.end(); it++) {
        ss << "Key: { " << it->first->to_string() << " }, Value: { " << it->second->to_string("", " ") << " }" << std::endl;
    }

    return ss.str();
}

Sqs::~Sqs() {
    for (auto it = map.begin(); it != map.end(); it++) {
        delete it->first;
        delete it->second;
    }
}
