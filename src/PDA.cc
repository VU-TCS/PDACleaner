#include "PDA.h"

#include <sstream>

PDA::PDA(std::string name, StateSet Q, SymbolSet Gamma,
        PDATransitionSet Delta, SymbolString Z, State q_0, StateSet F)
    : name(name), Q(Q), Gamma(Gamma), Delta(Delta), Z(Z), q_0(q_0), F(F) {}


StateSet * PDA::get_Q() {
    return &Q;
}

SymbolSet * PDA::get_Gamma() {
    return &Gamma;
}

PDATransitionSet * PDA::get_Delta() {
    return &Delta;
}

SymbolString PDA::get_Z() {
    return Z;
}

State PDA::get_q_0() {
    return q_0;
}

StateSet * PDA::get_F() {
    return &F;
}
    
bool PDA::equals(PDA const& other) const {
    return Q == other.Q
        && Gamma == other.Gamma
        && Delta == other.Delta
        && Z == other.Z
        && q_0 == other.q_0
        && F == other.F;
}

std::string PDA::to_string() const {
    std::stringstream ss;

    ss << name << " = {" << std::endl;
    ss << "\tQ = " << Q.to_string("", " ") << "," << std::endl;
    ss << "\tGamma = " << Gamma.to_string("", " ") << "," << std::endl;
    ss << "\tDelta = " << Delta.to_string("\t", "\n\t") << "," << std::endl;
    ss << "\tZ = " << Z << "," << std::endl;
    ss << "\tq_0 = " << q_0 << "," << std::endl;
    ss << "\tF = " << F.to_string("", " ") << std::endl;
    ss << "}" << std::endl;

    return ss.str();
}
