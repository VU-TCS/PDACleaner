#include "PDACleaner.h"

#include <iostream>

static PDA * simplify_PDA(PDA *P) {
    Bottom b_0;
    GeneratedState *q_0_hat = new GeneratedState("q_0_hat");
    GeneratedState *q_e = new GeneratedState("q_e");
    GeneratedState *q_f = new GeneratedState("q_f");

    // StateSet
    StateSet *Q = P->get_Q()->clone();
    Q->add(q_0_hat);
    Q->add(q_e);
    Q->add(q_f);

    // SymbolSet
    SymbolSet *Gamma = P->get_Gamma()->clone();
    Gamma->add(&b_0);

    // Initial stack string
    SymbolString *Z = new SymbolString();
    Z->append(&b_0);

    // TransitionSet
    PDATransitionSet *Delta = P->get_Delta()->clone();

    // Add necessary transitions to Delta
    SymbolString empty_string;
    
    PDATransition t(q_0_hat->clone(), empty_string.clone(), 0,
            P->get_q_0()->clone(), P->get_Z()->clone(), START_UP);
    Delta->add(&t);
    
    for (auto it = P->get_F()->begin(); it != P->get_F()->end(); it++) {
        PDATransition t((*it)->clone(), empty_string.clone(), 0,
                q_e->clone(), empty_string.clone(), CLOSE_DOWN);
        Delta->add(&t);
    }

    for (auto it = P->get_Gamma()->begin(); it != P->get_Gamma()->end(); it++) {
        SymbolString pop;
        pop.append(*it);
        PDATransition t(q_e->clone(), pop.clone(), 0,
                q_e->clone(), empty_string.clone(), CLOSE_DOWN);
        Delta->add(&t);
    }

    PDATransition r(q_e->clone(), Z->clone(), 0,
            q_f->clone(), empty_string.clone(), CLOSE_DOWN);
    Delta->add(&r);

    // Create new Final set with single final state
    StateSet *F = new StateSet();
    F->add(q_f);

    //Construct PDA
    PDA *P_0 = new PDA("P_0", Q, Gamma, Delta, Z, q_0_hat, F);

    // Safe to delete
    delete q_e;
    delete q_f;

    return P_0;
}

PDACleanerResult clean_PDA(PDA *P) {
    PDATransitionSet *U_1 = new PDATransitionSet;
    PDATransitionSet *U_2 = new PDATransitionSet;

    PDA *P_0 = simplify_PDA(P);

    PDACleanerResult result = { U_1, U_2, P_0 };
    return result;
}
