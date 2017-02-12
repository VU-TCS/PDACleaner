#ifndef SYMBOL_H
#define SYMBOL_H

#include <vector>

#include "Set.h"

/**
 * Represents a symbol to be used either as an input symbol or a stack symbol in a PDA.
 */
class Symbol {
    public:
        friend bool operator==(Symbol const& lhs, Symbol const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, Symbol const& self) {
            return strm << self.to_string();
        }

        virtual bool equals(Symbol const& other) const = 0;
        virtual std::size_t hash() const = 0;
        virtual std::string to_string() const = 0;
        virtual Symbol * clone() const = 0;
        virtual ~Symbol() {};
};  

/** 
 * The epsilon Symbol.
 */ 
class Epsilon : public Symbol {
    public:
        bool equals(Symbol const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        Symbol * clone() const;
};

/**
 * The bottom-of-the-stack Symbol required for simplified PDAs.
 */
class Bottom : public Symbol {
    public:
        bool equals(Symbol const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        Symbol * clone() const;
};

/**
 * A Symbol that consist of a single char.
 */
class Character : public Symbol {
    private:
        char c;

    public:
        Character(char c);
        bool equals(Symbol const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        Symbol * clone() const;
};

/**
 * A Symbol that consists of multiple chars.
 */
class Identifier : public Symbol {
    private:
        std::string identifier;

    public:
        Identifier(std::string identifier);
        bool equals(Symbol const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        Symbol * clone() const;
};

extern Epsilon EPSILON;

/**
 * The hasher required for the PointerSet.
 */
struct SymbolHash {
    inline std::size_t operator()(Symbol* const& s) const {
        return s->hash();
    }
};

/**
 * The equility checker required for the PointerSet.
 */
struct SymbolEq {
    inline bool operator()(Symbol const *lhs, Symbol const *rhs) const {
        return lhs->equals(*rhs);
    }
};

typedef Set<Symbol, SymbolHash, SymbolEq> SymbolSet;

/**
 * Represents a symbolstring to reprsent stack strings in a PDA and in a PDATransition.
 */
class SymbolString {
    private:
        std::vector<Symbol *> string;

    public:
        friend bool operator==(SymbolString const& lhs, SymbolString const& rhs) {
            return lhs.equals(rhs);
        }

        friend std::ostream& operator<<(std::ostream& strm, SymbolString const& self) {
            return strm << self.to_string();
        }

        void append(Symbol *s);
        Symbol * symbol_at(std::size_t i) const;
        std::size_t length() const;
        SymbolString * reverse() const;
        SymbolString * truncated(std::size_t k) const;
        bool equals(SymbolString const& other) const;
        std::size_t hash() const;
        std::string to_string() const;
        SymbolString * clone() const;
        ~SymbolString();
};

#endif /* SYMBOL_H */
