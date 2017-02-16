#include "Parser.h"

#include <sstream>
#include <fstream>
#include <stdexcept>

static void read_word(std::ifstream& input, std::string word) {
    std::string read;
    input >> std::skipws >> read;

    if (read != word) {
        std::stringstream ss;
        ss << "Expected \"" << word << "\". Read \"" << read << "\".";
        throw std::invalid_argument(ss.str());
    }
}

static void read_char(std::ifstream& input, char c) {
    char read;
    input >> std::skipws >> read;

    if (read != c) {
        std::stringstream ss;
        ss << "Expected '" << c << "'. Read '" << read << "'.";
        throw std::invalid_argument(ss.str());
    }
}

static char read_input_symbol(std::ifstream& input) {
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

static Symbol * read_Symbol(std::ifstream& input) {
    char c;
    input >> c;
    return new Character(c);
}

static SymbolString * read_SymbolString(std::ifstream& input, SymbolSet *Gamma) {
    read_char(input, '"');
    
    SymbolString *str = new SymbolString;
    char c;

    while ((c = (input >> std::ws).peek()) != '"') {
        Symbol *s = read_Symbol(input);
        str->append(s);

        if (!Gamma->contains(s)) {
            //throw std::invalid_argument("Symbol " + s->to_string() + " not contained in Gamma.");
            
            // Instead of throwing an exception, silently add the Symbol to Gamma.
            // This just makes writing the input files (especially large PDAs) much easier.
            Gamma->add(s);
        }

        delete s;
    }

    read_char(input, '"');
    return str;
}

static State * read_State(std::ifstream& input) {
    int i;
    input >> i;
    return new SimpleState(i);
}

static StateSet * read_F(std::ifstream& input, StateSet *Q) {
    read_char(input, 'F');
    read_char(input, '=');
    read_char(input, '{');

    StateSet *F = new StateSet;
    int i;

    while ((i = (input >> std::ws).peek()) != '}') {
        State *s = read_State(input);
        F->add(s);

        if (!Q->contains(s)) {
            //throw std::invalid_argument("Final state " + s->to_string() + " not contained in Q.");
            
            // Instead of throwing an exception, silently add the State to Q.
            // This just makes writing the input files (especially large PDAs) much easier.
            Q->add(s);
        }

        delete s;
    }

    read_char(input, '}');
    return F;
}

static State * read_q_0(std::ifstream& input, StateSet *Q) {
    read_word(input, "q_0");
    read_char(input, '=');
    
    State *q_0 = read_State(input);
    if (!Q->contains(q_0)) {
        //throw std::invalid_argument("Initial state q_0 = " + q_0->to_string() + " not contained in Q.");
            
        // Instead of throwing an exception, silently add the State to Q.
        // This just makes writing the input files (especially large PDAs) much easier.
        Q->add(q_0);
    }

    return q_0;
}

static SymbolString * read_Z(std::ifstream& input, SymbolSet *Gamma) {
    read_char(input, 'Z');
    read_char(input, '=');
    return read_SymbolString(input, Gamma);
}

static PDATransition * read_PDATransition(std::ifstream& input, StateSet *Q, SymbolSet *Gamma) {
    read_char(input, '(');
    
    State *q = read_State(input);
    if (!Q->contains(q)) {
        //throw std::invalid_argument("State " + q->to_string() + " not contained in Q.");
        
        // Instead of throwing an exception, silently add the State to Q.
        // This just makes writing the input files (especially large PDAs) much easier.
        Q->add(q);
    }
    read_char(input, ',');

    SymbolString *sigma = read_SymbolString(input, Gamma);
    read_char(input, ',');

    char symbol = read_input_symbol(input);
    read_char(input, ',');
    
    State *r = read_State(input);
    if (!Q->contains(r)) {
        //throw std::invalid_argument("State " + r->to_string() + " not contained in Q.");
        
        // Instead of throwing an exception, silently add the State to Q.
        // This just makes writing the input files (especially large PDAs) much easier.
        Q->add(r);
    }
    read_char(input, ',');
    
    SymbolString *tau = read_SymbolString(input, Gamma);
    read_char(input, ')');

    PDATransition *t = new PDATransition(q, sigma, symbol, r, tau);

    delete q;
    delete sigma;
    delete r;
    delete tau;

    return t;
}

static PDATransitionSet * read_Delta(std::ifstream& input, StateSet *Q, SymbolSet *Gamma) {
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

static SymbolSet * read_Gamma(std::ifstream& input) {
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

static StateSet * read_Q(std::ifstream& input) {
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

static std::string read_name(std::ifstream& input) {
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

    PDA *P = new PDA(name, Q, Gamma, Delta, Z, q_0, F);

    delete Q;
    delete Gamma;
    delete Delta;
    delete Z;
    delete q_0;
    delete F;

    return P;
}
