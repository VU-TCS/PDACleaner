#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>

#include "Set.h"

/**
 * Represents a symbol to be used as a stack symbol in a PDA.
 */
class Symbol {
    private:
        char c;

    public:
        friend bool operator==(Symbol const& lhs, Symbol const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, Symbol const& self) {
            return strm << self.to_string();
        }

        /**
         * A Symbol should not be instantiated with the null character.
         * This character has a special meaning. See he definition of
         * EPSILON below.
         */
        Symbol(char c);
        bool equals(Symbol const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
};

/**
 * The hasher required for the Set.
 */
struct SymbolHash {
    inline std::size_t operator()(Symbol const& s) const {
        return s.hash();
    }
};

/**
 * The equility checker required for the Set.
 */
struct SymbolEq {
    inline bool operator()(Symbol const lhs, Symbol const rhs) const {
        return lhs.equals(rhs);
    }
};

typedef Set<Symbol, SymbolHash, SymbolEq> SymbolSet;

/**
 * Represents a symbolstring to reprsent stack strings in a PDA and in a PDATransition.
 */
class SymbolString {
    private:
        std::vector<Symbol> string;

    public:
        friend bool operator==(SymbolString const& lhs, SymbolString const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, SymbolString const& self) {
            return strm << self.to_string();
        }

        void append(Symbol s);
        Symbol symbol_at(std::size_t i) const;
        std::size_t length() const;
        bool equals(SymbolString const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
};

/**
 * Represents the epsilon symbol.
 */
extern Symbol EPSILON;

#endif /* SYMBOL_H */
