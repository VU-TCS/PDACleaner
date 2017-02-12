#include "PDATransition.h"

#include <sstream>

PDATransition::PDATransition(State *q, SymbolString *sigma, char symbol, State *r, SymbolString *tau, ExtensionStatus extension) {
    this->q = q->clone();
    this->sigma = sigma->clone();
    this->symbol = symbol;
    this->r = r->clone();
    this->tau = tau->clone();
    this->extension = extension;
}

State * PDATransition::get_q() {
    return q;
}

SymbolString * PDATransition::get_sigma() {
    return sigma;
}

char PDATransition::get_symbol() {
    return symbol;
}

State * PDATransition::get_r() {
    return r;
}

SymbolString * PDATransition::get_tau() {
    return tau;
}

ExtensionStatus PDATransition::get_ext_status() {
    return extension;
}

bool PDATransition::equals(PDATransition const& other) const {
    return q->equals(*other.q)
        && sigma->equals(*other.sigma)
        && symbol == other.symbol
        && r->equals(*other.r)
        && tau->equals(*other.tau)
        && extension == other.extension;
}

std::size_t PDATransition::hash() const {
    std::size_t res = 17;
    res = res * 19 * q->hash();
    res = res * 23 * sigma->hash();
    res = res * 29 * std::hash<char>{}(symbol);
    res = res * 31 * r->hash();
    res = res * 37 * tau->hash();
    res = res * 7 * std::hash<short>{}(extension);
    return res;
}

std::string PDATransition::to_string() const {
    std::stringstream ss;

    ss << "(" << *q << ", " << *sigma << ", \"" << symbol <<
        "\", " << *r << ", " << *tau << ")";

    return ss.str();
}

PDATransition * PDATransition::clone() const {
    return new PDATransition(q, sigma, symbol, r, tau, extension);
}

PDATransition::~PDATransition() {
    delete q;
    delete sigma;
    delete r;
    delete tau;
}
