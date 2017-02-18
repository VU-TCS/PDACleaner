#include "PDA.h"

#include <sstream>

PDA::PDA(std::string name, StateSet *Q, SymbolSet *Gamma,
        PDATransitionSet *Delta, SymbolString *Z, State *q_0, StateSet *F) {
    this->name = name;
    this->Q = Q->clone();
    this->Gamma = Gamma->clone();
    this->Delta = Delta->clone();
    this->Z = Z->clone();
    this->q_0 = q_0->clone();
    this->F = F->clone();
}

StateSet * PDA::get_Q() {
    return Q;
}

SymbolSet * PDA::get_Gamma() {
    return Gamma;
}

PDATransitionSet * PDA::get_Delta() {
    return Delta;
}

SymbolString * PDA::get_Z() {
    return Z;
}

State * PDA::get_q_0() {
    return q_0;
}

StateSet * PDA::get_F() {
    return F;
}
    
bool PDA::equals(PDA const& other) const {
    return Q->equals(*other.Q)
        && Gamma->equals(*other.Gamma)
        && Delta->equals(*other.Delta)
        && Z->equals(*other.Z)
        && q_0->equals(*other.q_0)
        && F->equals(*other.F);
}

std::string PDA::to_string() const {
    std::stringstream ss;

    ss << name << " = {\n";
    ss << "\tQ = " << Q->to_string("", " ") << ",\n";
    ss << "\tGamma = " << Gamma->to_string("", " ") << ",\n";
    ss << "\tDelta = " << Delta->to_string("\t", "\n\t") << ",\n";
    ss << "\tZ = " << *Z << ",\n";
    ss << "\tq_0 = " << *q_0 << ",\n";
    ss << "\tF = " << F->to_string("", " ") << "\n";
    ss << "}";

    return ss.str();
}

PDA * PDA::clone() const {
    return new PDA(name, Q, Gamma, Delta, Z, q_0, F);
}

PDA::~PDA() {
    delete Q;
    delete Gamma;
    delete Delta;
    delete Z;
    delete q_0;
    delete F;
}
