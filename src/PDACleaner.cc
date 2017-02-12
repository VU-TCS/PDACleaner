#include "PDACleaner.h"

#include <iostream>

#include "NFA.h"
#include "Sqs.h"
#include "BMap.h"

/**
 * Establishes a path (y, a, z) in N and returns State y, which may be a newly created State.
 * Corresponds to Algorithm 2 (step 4 of forward) described in the paper.
 */
static State * forward_4(SymbolString *a, State *z, NFA *N, BMap *b_map) {
    int k = a->length();

    if (k == 0)
        return z->clone();
    
    for (auto it = N->get_Delta()->begin(); it != N->get_Delta()->end(); it++) {
        State *q = (*it)->get_q();
        Symbol *symbol = (*it)->get_symbol();
        State *r = (*it)->get_r();

        if (symbol->equals(*a->symbol_at(k - 1)) && r->equals(*z)) {
            SymbolString *truncated = a->truncated(1);
            State *y = forward_4(truncated, q, N, b_map);
            delete truncated;

            return y;
        }
    }

    State *n_1 = StateGenerator::get_state();
    State *n_2 = z->clone();

    for (int i = k - 1; i >= 0; i--) {
        NFATransition t(n_1, a->symbol_at(i), n_2);

        N->get_Q()->add(n_1);
        b_map->get(n_1)->add(n_1);
        N->get_Delta()->add(&t);
        
        delete n_2;
        n_2 = n_1;

        if (k > 1)
            n_1 = StateGenerator::get_state();
    }

    return n_1;
}

/**
 * Determines the largest set of States in N, such that each of them can reach
 * a state in the specified set via a path of epsilon transitions only.
 */
static StateSet * backwards_epsilon_closure(NFA *N, StateSet *set, BMap *b_map) {
    StateSet *set_prime = set->clone();

    for (auto it = set->begin(); it != set->end(); it++) {
        set_prime->add_all(b_map->get(*it));
    }
    
    return set_prime;
}

/**
 * Determines the sqs set for State q and SymbolString s.
 * Enters this set into the pecified Sqs map.
 */
static void determine_Sqs(Sqs *sqs_map, State *q, SymbolString *s, NFA *N, BMap *b_map) {
    SymbolString *s_reversed = s->reverse();
    StateSet *set = sqs_map->get(q, s);
    set->add(q);

    for (int i = s_reversed->length() - 1; i >= 0; i--) {
        StateSet *set_eps = backwards_epsilon_closure(N, set, b_map);

        set->clear();

        for (auto it = N->get_Delta()->begin(); it != N->get_Delta()->end(); it++) {
            State *q = (*it)->get_q();
            Symbol *symbol = (*it)->get_symbol();
            State *r = (*it)->get_r();

            if (symbol->equals(*s_reversed->symbol_at(i)) && set_eps->contains(r)) {
                set->add(q);
            }
        }

        delete set_eps;
    }

    delete s_reversed;
}

/**
 * Detects unreachable transitions in P_0.
 * Corresponds to Algorithm 1 (Procedure forward) described in the paper.
 */
static void forward(PDA *P_0, NFA *N, PDATransitionSet *U_1, Sqs *sqs_map, BMap *b_map) {
    for (auto theta = P_0->get_Delta()->begin(); theta != P_0->get_Delta()->end(); theta++) {
        State *q = (*theta)->get_q();
        SymbolString *sigma = (*theta)->get_sigma();
        SymbolString *tau = (*theta)->get_tau();
        State *r = (*theta)->get_r();

        if (!N->get_Q()->contains(q))
            continue;

        determine_Sqs(sqs_map, q, sigma, N, b_map);

        StateSet *sqs = sqs_map->get(q, sigma);
        if (sqs->size() == 0)
            continue;

        SymbolString *tau_reversed = tau->reverse();
        
        if (U_1->contains(*theta)) {
            U_1->remove(*theta);
            
            State *tmp = forward_4(tau_reversed, r, N, b_map);
            delete tmp;

            N->get_Q()->add(r);
            b_map->get(r)->add(r);
            N->get_F()->add(r);
        }

        NFATransitionSet *Delta = N->get_Delta();
        State *y = forward_4(tau_reversed, r, N, b_map);

        for (auto x = sqs->begin(); x != sqs->end(); x++) {
            NFATransition t(*x, &EPSILON, y);
            
            if (!Delta->contains(&t)) {
                Delta->add(&t);

                // Merge BMap entries.
                for (auto b_it = b_map->begin(); b_it != b_map->end(); b_it++) {
                    StateSet *b_set = b_it->second;

                    if (!b_set->contains(y))
                        continue;

                    b_set->add_all(b_map->get(*x));
                }
            }
        }

        delete y;
        delete tau_reversed;
    }
}

/**
 * Builds the NFA N that represents all reachable configurations in P_0.
 * Also removes reachable transitions from U_1, which in the end will
 * contain all unreachable transitions of P_0.
 */
static NFA * build_NFA(PDA *P_0, PDATransitionSet *U_1, Sqs *sqs_map) {
    BMap *b_map = new BMap();
    
    // Initial state of the NFA
    GeneratedState m_0("m_0");
    
    // Stateset of the NFA
    StateSet Q;
    Q.add(&m_0);
    b_map->get(&m_0)->add(&m_0);
    Q.add(P_0->get_q_0());
    b_map->get(P_0->get_q_0())->add(P_0->get_q_0());

    // The NFA's input symbol's are P_0's stack symbols
    SymbolSet *Sigma = P_0->get_Gamma()->clone();

    // The NFA's transitions
    NFATransitionSet Delta;
    
    // Initially contains only one transition
    Bottom b_0;
    NFATransition t(&m_0, &b_0, P_0->get_q_0());
    Delta.add(&t);

    // The NFA's final states
    StateSet F;
    F.add(P_0->get_q_0());

    NFA *old_N = nullptr;
    NFA *N = new NFA(&Q, Sigma, &Delta, &m_0, &F);
    delete Sigma;

    do {
        if (old_N != nullptr)
            delete old_N;

        old_N = N->clone();
        sqs_map->clear();
        forward(P_0, N, U_1, sqs_map, b_map);
    } while (!N->equals(*old_N));

    delete b_map;
    delete old_N;
    return N;
}

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
    Sqs *sqs_map = new Sqs();
    PDA *P_0 = simplify_PDA(P);

    PDATransitionSet *U_1 = P_0->get_Delta()->clone();
    PDATransitionSet *U_2 = new PDATransitionSet();
    
    NFA *N = build_NFA(P_0, U_1, sqs_map);

    PDACleanerResult result = { U_1, U_2, P_0 };
    
    delete sqs_map;
    delete N;
    return result;
}
