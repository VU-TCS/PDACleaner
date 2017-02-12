#include "NFA.h"

#include <sstream>

NFA::NFA(StateSet *Q, SymbolSet *Sigma, NFATransitionSet *Delta, State *q_0, StateSet *F) {
    this->Q = Q->clone();
    this->Sigma = Sigma->clone();
    this->Delta = Delta->clone();
    this->q_0 = q_0->clone();
    this->F = F->clone();
}
    
StateSet * NFA::get_Q() {
    return Q;
}

SymbolSet * NFA::get_Sigma() {
    return Sigma;
}

NFATransitionSet * NFA::get_Delta() {
    return Delta;
}

State * NFA::get_q_0() {
    return q_0;
}

StateSet * NFA::get_F() {
    return F;
}
    
bool NFA::equals(NFA const& other) const {
    return Q->equals(*other.Q)
        && Sigma->equals(*other.Sigma)
        && Delta->equals(*other.Delta)
        && q_0->equals(*other.q_0)
        && F->equals(*other.F);
}

std::string NFA::to_string() const {
    std::stringstream ss;

    ss << "{" << std::endl;
    ss << "\tQ = " << Q->to_string("", " ") << "," << std::endl;
    ss << "\tSigma = " << Sigma->to_string("", " ") << "," << std::endl;
    ss << "\tDelta = " << Delta->to_string("\t", "\n\t") << "," << std::endl;;
    ss << "\tq_0 = " << *q_0 << "," << std::endl;
    ss << "\tF = " << F->to_string("", " ") << "," << std::endl;
    ss << "}";

    return ss.str();
}

NFA * NFA::clone() const {
    return new NFA(Q, Sigma, Delta, q_0, F);
}

NFA::~NFA() {
    delete Q;
    delete Sigma;
    delete Delta;
    delete q_0;
    delete F;
}
