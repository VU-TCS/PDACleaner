#include "NFATransition.h"

#include <sstream>

NFATransition::NFATransition(State *q, Symbol *symbol, State *r) {
    this->q = q->clone();
    this->symbol = symbol->clone();
    this->r = r->clone();
}

bool NFATransition::equals(NFATransition const& other) const {
    return q->equals(*other.q)
        && symbol->equals(*other.symbol)
        && r->equals(*other.r);
}

std::size_t NFATransition::hash() const {
    std::size_t res = 17;
    res = res * 19 * q->hash();
    res = res * 29 * symbol->hash();
    res = res * 31 * r->hash();
    return res;
}

std::string NFATransition::to_string() const {
    std::stringstream ss;

    ss << "{" << *q << ", " << *symbol << ", " << *r << "}";

    return ss.str();
}

NFATransition * NFATransition::clone() const {
    return new NFATransition(q, symbol, r);
}

NFATransition::~NFATransition() {
    delete q;
    delete symbol;
    delete r;
}
