#include "NFA.h"

#include <sstream>

NFA::NFA(StateSet Q, SymbolSet Sigma, NFATransitionSet Delta, State q_0, StateSet F)
    : Q(Q), Sigma(Sigma), Delta(Delta), q_0(q_0), F(F) {}

bool NFA::equals(NFA const& other) const {
    return Q == other.Q
        && Sigma == other.Sigma
        && Delta == other.Delta
        && q_0 == other.q_0
        && F == other.F;
}

std::string NFA::to_string() const {
    std::stringstream ss;

    ss << "{" << std::endl;
    ss << "\tQ = " << Q.to_string("", " ") << "," << std::endl;
    ss << "\tSigma = " << Sigma.to_string("", " ") << "," << std::endl;
    ss << "\tDelta = " << Delta.to_string("\t", "\n\t") << "," << std::endl;;
    ss << "\tq_0 = " << q_0 << "," << std::endl;
    ss << "\tF = " << F.to_string("", " ") << "," << std::endl;
    ss << "}" << std::endl;

    return ss.str();
}
