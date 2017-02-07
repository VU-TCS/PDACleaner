#include "Symbol.h"

#include <sstream>

/*** EPSILON ***/

Symbol EPSILON(0);


/*** Symbol ***/

Symbol::Symbol(char c) {
    this->c = c;
}

bool Symbol::equals(Symbol const& other) const {
    return c == other.c;
}

std::size_t Symbol::hash() const {
    return std::hash<char>{}(c);
}

std::string Symbol::to_string() const {
    if (this->equals(EPSILON))
        return "\\epsilon";

    return std::string(1, c);
}


/*** SymbolString ***/

void SymbolString::append(Symbol s) {
    string.push_back(s);
}

Symbol SymbolString::symbol_at(std::size_t i) const {
    return string.at(i);
}

std::size_t SymbolString::length() const {
    return string.size();
}

bool SymbolString::equals(SymbolString const& other) const {
    if (length() != other.length())
        return false;

    for (std::size_t i = 0; i < length(); i++) {
        if (!symbol_at(i).equals(other.symbol_at(i)))
            return false;
    }

    return true;
}

std::size_t SymbolString::hash() const {
    return std::hash<std::string>{}(to_string());
}

std::string SymbolString::to_string() const {
    std::stringstream ss;

    ss << "\"";
    for (auto it = string.begin(); it != string.end(); it++) {
        ss << *it;
    }
    ss << "\"";

    return ss.str();
}

