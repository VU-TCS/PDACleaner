#include "PDATransition.h"

#include <sstream>

PDATransition::PDATransition(State q, SymbolString sigma, char symbol, State r, SymbolString tau, ExtensionStatus extension)
    : q(q), sigma(sigma), symbol(symbol), r(r), tau(tau), extension(extension) {}

bool PDATransition::equals(PDATransition const& other) const {
    return q == other.q
        && sigma == other.sigma
        && symbol == other.symbol
        && r == other.r
        && tau == other.tau
        && extension == other.extension;
}

std::size_t PDATransition::hash() const {
    std::size_t res = 17;
    res = res * 19 * q.hash();
    res = res * 23 * sigma.hash();
    res = res * 29 * std::hash<char>{}(symbol);
    res = res * 31 * r.hash();
    res = res * 37 * tau.hash();
    res = res * 7 * std::hash<short>{}(extension);
    return res;
}

std::string PDATransition::to_string() const {
    std::stringstream ss;

    ss << "(" << q << ", " << sigma << ", \"" << symbol <<
        "\", " << r << ", " << tau << ")";

    return ss.str();
}
