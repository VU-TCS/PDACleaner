#include "NFATransition.h"

#include <sstream>

NFATransition::NFATransition(State q, Symbol symbol, State r)
    : q(q), symbol(symbol), r(r) {}

bool NFATransition::equals(NFATransition const& other) const {
    return q == other.q
        && symbol == other.symbol
        && r == other.r;
}

std::size_t NFATransition::hash() const {
    std::size_t res = 17;
    res = res * 19 * q.hash();
    res = res * 29 * symbol.hash();
    res = res * 31 * r.hash();
    return res;
}

std::string NFATransition::to_string() const {
    std::stringstream ss;

    ss << "{" << q << ", " << symbol << ", " << r << "}";

    return ss.str();
}
