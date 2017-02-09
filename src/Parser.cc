#include "Parser.h"

#include <sstream>
#include <fstream>
#include <stdexcept>

void read_word(std::ifstream& input, std::string word) {
    std::string read;
    input >> std::skipws >> read;

    if (read != word) {
        std::stringstream ss;
        ss << "Expected \"" << word << "\". Read \"" << read << "\".";
        throw std::invalid_argument(ss.str());
    }
}

void read_char(std::ifstream& input, char c) {
    char read;
    input >> std::skipws >> read;

    if (read != c) {
        std::stringstream ss;
        ss << "Expected '" << c << "'. Read '" << read << "'.";
        throw std::invalid_argument(ss.str());
    }
}

char read_input_symbol(std::ifstream& input) {
    read_char(input, '"');
    
    char c;
    if ((c = (input >> std::ws).peek()) != '"') {
        input >> c;
    } else {
        c = 0;
    }

    read_char(input, '"');
    return c;
}

Symbol * read_Symbol(std::ifstream& input) {
    char c;
    input >> c;
    return new Character(c);
}

SymbolString * read_SymbolString(std::ifstream& input, SymbolSet *Gamma) {
    read_char(input, '"');
    
    SymbolString *str = new SymbolString;
    char c;

    while ((c = (input >> std::ws).peek()) != '"') {
        Symbol *s = read_Symbol(input);
        str->append(s);

        if (!Gamma->contains(s)) {
            throw std::invalid_argument("Symbol " + s->to_string() + " not contained in Gamma.");
        }

        delete s;
    }

    read_char(input, '"');
    return str;
}

State * read_State(std::ifstream& input) {
    int i;
    input >> i;
    return new SimpleState(i);
}

StateSet * read_F(std::ifstream& input, StateSet *Q) {
    read_char(input, 'F');
    read_char(input, '=');
    read_char(input, '{');

    StateSet *F = new StateSet;
    int i;

    while ((i = (input >> std::ws).peek()) != '}') {
        State *s = read_State(input);
        F->add(s);

        if (!Q->contains(s)) {
            throw std::invalid_argument("Final state " + s->to_string() + " not contained in Q.");
        }

        delete s;
    }

    read_char(input, '}');
    return F;
}

State * read_q_0(std::ifstream& input, StateSet *Q) {
    read_word(input, "q_0");
    read_char(input, '=');
    
    State *q_0 = read_State(input);
    if (!Q->contains(q_0)) {
        throw std::invalid_argument("Initial state q_0 = " + q_0->to_string() + " not contained in Q.");
    }

    return q_0;
}

SymbolString * read_Z(std::ifstream& input, SymbolSet *Gamma) {
    read_char(input, 'Z');
    read_char(input, '=');
    return read_SymbolString(input, Gamma);
}

PDATransition * read_PDATransition(std::ifstream& input, StateSet *Q, SymbolSet *Gamma) {
    read_char(input, '(');
    
    State *q = read_State(input);
    if (!Q->contains(q)) {
        throw std::invalid_argument("State " + q->to_string() + " not contained in Q.");
    }
    read_char(input, ',');

    SymbolString *sigma = read_SymbolString(input, Gamma);
    read_char(input, ',');

    char symbol = read_input_symbol(input);
    read_char(input, ',');
    
    State *r = read_State(input);
    if (!Q->contains(r)) {
        throw std::invalid_argument("State " + r->to_string() + " not contained in Q.");
    }
    read_char(input, ',');
    
    SymbolString *tau = read_SymbolString(input, Gamma);
    read_char(input, ')');

    return new PDATransition(q, sigma, symbol, r, tau);
}

PDATransitionSet * read_Delta(std::ifstream& input, StateSet *Q, SymbolSet *Gamma) {
    read_word(input, "Delta");
    read_char(input, '=');
    read_char(input, '{');

    PDATransitionSet *Delta = new PDATransitionSet;
    char c;

    while ((c = (input >> std::ws).peek()) != '}') {
        PDATransition *t = read_PDATransition(input, Q, Gamma);
        Delta->add(t);
        delete t;
    }

    read_char(input, '}');
    return Delta;
}

SymbolSet * read_Gamma(std::ifstream& input) {
    read_word(input, "Gamma");
    read_char(input, '=');
    read_char(input, '{');

    SymbolSet *Gamma = new SymbolSet;
    char c;

    while ((c = (input >> std::ws).peek()) != '}') {
        Symbol *s = read_Symbol(input);
        Gamma->add(s);
        delete s;
    }

    read_char(input, '}');
    return Gamma;
}

StateSet * read_Q(std::ifstream& input) {
    read_char(input, 'Q');
    read_char(input, '=');
    read_char(input, '{');

    StateSet *Q = new StateSet;
    int i;

    while ((i = (input >> std::ws).peek()) != '}') {
        State *s = read_State(input);
        Q->add(s);
        delete s;
    }

    read_char(input, '}');
    return Q;
}

std::string read_name(std::ifstream& input) {
    std::string name;
    input >> std::skipws >> name;
    return name;
}

PDA * parse_PDA(std::ifstream& input) {
    std::string name = read_name(input);
    read_char(input, '=');
    read_char(input, '{');
    
    StateSet *Q = read_Q(input);
    read_char(input, ',');

    SymbolSet *Gamma = read_Gamma(input);
    read_char(input, ',');
    
    PDATransitionSet *Delta = read_Delta(input, Q, Gamma);
    read_char(input, ',');
    
    SymbolString *Z = read_Z(input, Gamma);
    read_char(input, ',');

    State *q_0 = read_q_0(input, Q);
    read_char(input, ',');

    StateSet *F = read_F(input, Q);
    read_char(input, '}');

    return new PDA(name, Q, Gamma, Delta, Z, q_0, F);
}
