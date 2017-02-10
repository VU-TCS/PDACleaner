#include "PDACleaner.h"

#include <iostream>

static PDA * simplify_PDA(PDA *P) {
    Bottom b_0;
    GeneratedState q_0_hat("q_0_hat");
    GeneratedState q_e("q_e");
    GeneratedState q_f("q_f");
    SymbolString Z;
    Z.append(&b_0);
    StateSet F;
    F.add(&q_f);

    PDA *P_0 = new PDA("P_0", P->get_Q(), P->get_Gamma(), P->get_Delta(), &Z, &q_0_hat, &F);

    // StateSet
    P_0->get_Q()->add(&q_0_hat);
    P_0->get_Q()->add(&q_e);
    P_0->get_Q()->add(&q_f);

    // SymbolSet
    P_0->get_Gamma()->add(&b_0);

    // Add necessary transitions to Delta
    SymbolString empty_string;
    
    PDATransition t(&q_0_hat, &empty_string, 0, P->get_q_0(), P->get_Z(), START_UP);
    P_0->get_Delta()->add(&t);
    
    for (auto it = P->get_F()->begin(); it != P->get_F()->end(); it++) {
        PDATransition t(*it, &empty_string, 0, &q_e, &empty_string, CLOSE_DOWN);
        P_0->get_Delta()->add(&t);
    }

    for (auto it = P->get_Gamma()->begin(); it != P->get_Gamma()->end(); it++) {
        SymbolString pop;
        pop.append(*it);
        PDATransition t(&q_e, &pop, 0, &q_e, &empty_string, CLOSE_DOWN);
        P_0->get_Delta()->add(&t);
    }

    PDATransition r(&q_e, &Z, 0, &q_f, &empty_string, CLOSE_DOWN);
    P_0->get_Delta()->add(&r);

    return P_0;
}

PDACleanerResult clean_PDA(PDA *P) {
    PDATransitionSet *U_1 = new PDATransitionSet;
    PDATransitionSet *U_2 = new PDATransitionSet;

    PDA *P_0 = simplify_PDA(P);

    PDACleanerResult result = { U_1, U_2, P_0 };
    return result;
}
