#include "PDACleaner.h"

#include "NFA.h"
#include "Sqs.h"
#include "BMap.h"
#include "StateVisitMap.h"

/**
 * Removes all States from the StateSet and the final StateSet of PDA P
 * that do not occur in any PDATransition in P.
 */
static void remove_unused_states(PDA *P) {
    P->get_Q()->clear();
    
    for (auto theta = P->get_Delta()->begin(); theta != P->get_Delta()->end(); theta++) {
       State *q = (*theta)->get_q();
       State *r = (*theta)->get_r();

       P->get_Q()->add(q);
       P->get_Q()->add(r);
    }

    P->get_F()->retain_all(P->get_Q());
}

/**
 * Removes all PDATransitions marked as extensions from the set U.
 */
static void remove_extensions(PDATransitionSet *U) {
    for (auto it = U->begin(); it != U->end();) {
        if ((*it)->get_ext_status() != NO_EXTENSION) {
            it = U->remove(*it);
        } else {
            it++;
        }
    }
}

/**
 * Helper function to recursively find the epsilon transitions mentioned in the description
 * of add_eps_transitions() (see below).
 */
static int add_eps_transitions_rec (std::size_t s_pos, State *x, SymbolString *sigma, State *q,
        NFA *N, NFATransitionSet *E, NFATransitionSet *traversed, StateVisitMap *visited) {

    if (visited->has_key(x)) {
        return visited->get(x);
    }

    int path = 0;

    for (auto it = N->get_Delta()->begin(); it != N->get_Delta()->end(); it ++) {
        if (!(*it)->get_q()->equals(*x)) continue;

        Symbol *a = (*it)->get_symbol();
        State *r = (*it)->get_r(); 

        if (a->equals(EPSILON)) {
            if (s_pos == 0) continue;
            if (traversed->contains(*it)) continue;

            traversed->add(*it);
            int res = add_eps_transitions_rec(s_pos, r, sigma, q, N, E, traversed, visited);

            traversed->remove(*it);

            if (res > 0) {
                E->add(*it);
            }

            path += res;
        }

        if (sigma->length() > s_pos && a->equals(*sigma->symbol_at(s_pos))) {
            s_pos++;
            NFATransitionSet traversed_new;
            StateVisitMap visited_new;

            int res = add_eps_transitions_rec(s_pos, r, sigma, q, N, E, &traversed_new, &visited_new);

            s_pos--;
            path += res;
        }
    }

    if (s_pos >= sigma->length() && x->equals(*q)) {
        path = 1;
    }

    visited->put(x, path);
    return path;
}

/**
 * Find and add all epsilon transitions to E that occur in any path
 * from x to q in N by sigma^R. The only restriction being that the first transition
 * in the path should not be an epsilon transition.
 *
 * Corresponds to Algorithm 3 - Step 3 in the paper.
 */
static void add_eps_transitions(State *x,
        SymbolString *sigma, State *q, NFA *N, NFATransitionSet *E) {

    // The traversed set and visited map are needed in order to detect
    // cycles during the recursive operation of finding the epsilon transitions.
    NFATransitionSet traversed;
    StateVisitMap visited;

    add_eps_transitions_rec(0, x, sigma, q, N, E, &traversed, &visited);
}

/**
 * Helper function to recursively find the epsilon-free path mentioned in the
 * description of find_finishing_path() (see below).
 */
static int find_finishing_path_rec(State *y, SymbolString **tau, State **r, NFA *N) {
    if (N->get_F()->contains(y)) {
        *r = y->clone();
        return 1;
    }

    for (auto it = N->get_Delta()->begin(); it != N->get_Delta()->end(); it ++) {
        State *q = (*it)->get_q();

        if (!q->equals(*y)) continue;

        Symbol *a = (*it)->get_symbol();
        if (a->equals(EPSILON)) continue;

        State *y_1 = (*it)->get_r();

        (*tau)->append(a);

        int res = find_finishing_path_rec(y_1, tau, r, N);

        if (res > 0) {
            return 1;
        }

        (*tau)->truncate(1);
    }

    return 0;
}

/**
 * For a given State y, determine SymbolString tau and State r such that am epsilon-free path
 * (y, tau^R, r) exists in N.
 *
 * Corresponds to Algorithm 3 - Step 2 mentioned in the paper.
 */
static void find_finishing_path(State *y, SymbolString **tau, State **r, NFA *N) {
    *tau = new SymbolString();
    find_finishing_path_rec(y, tau, r, N);
    SymbolString *tau_rev = (*tau)->reversed();
    delete *tau;
    *tau = tau_rev;
}

/**
 * Determines which transitions in P_1 are non-finishing.
 * Corresponds to Algorithm 3 (procedure backward) in the paper.
 */
static void backward(PDA *P_1, NFA *N, PDATransitionSet *U_2,
        NFATransitionSet *E, NFATransitionSet *G, Sqs *sqs_map) {

    PDATransitionSet *Delta = P_1->get_Delta();
    NFATransitionSet *E_minus_G = E->difference(G);

    while (!E_minus_G->is_empty()) {
        // Step 1
        NFATransition *x_eps_y = *E_minus_G->begin();
        State *x = x_eps_y->get_q();
        State *y = x_eps_y->get_r();

        G->add(x_eps_y);

        // Step 2
        SymbolString *tau = nullptr;
        State *r = nullptr;
        find_finishing_path(y, &tau, &r, N);

        // Step 3
        for (auto theta = Delta->begin(); theta != Delta->end(); theta++) {
            if (!(*theta)->get_r()->equals(*r)) continue;
            if (!(*theta)->get_tau()->equals(*tau)) continue;

            State *q = (*theta)->get_q();
            SymbolString *sigma = (*theta)->get_sigma();

            // Step 3.1
            if (!sqs_map->get(q, sigma)->contains(x)) continue;

            // Step 3.2
            U_2->remove(*theta);

            // Step 3.3
            if (sigma->length() > 0) {
                SymbolString *sigma_rev = sigma->reversed();
                add_eps_transitions(x, sigma_rev, q, N, E);
                delete sigma_rev;
            }
        }

        delete tau;
        delete r;
        
        delete E_minus_G;
        E_minus_G = E->difference(G);
    }

    delete E_minus_G;
}

/**
 * Makes preparations for the backward procedure.
 */
static void non_finishing(PDA *P_1, NFA *N, Sqs *sqs_map, PDATransitionSet *U_2) {
    State *m_0 = N->get_q_0();
    State *q_f = *P_1->get_F()->begin();
    NFATransition e(m_0, &EPSILON, q_f);

    if (!N->get_Delta()->contains(&e)) {
        // The language is empty, so all transitions are useless.
        return;
    }

    NFATransitionSet E, G;
    E.add(&e);

    backward(P_1, N, U_2, &E, &G, sqs_map);
}

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

        if (i > 0)
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
    SymbolString *s_reversed = s->reversed();
    StateSet *set = sqs_map->get(q, s);
    set->clear();
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

        SymbolString *tau_reversed = tau->reversed();
        
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
    
    PDATransition t(&q_0_hat, &empty_string, "", P->get_q_0(), P->get_Z(), START_UP);
    P_0->get_Delta()->add(&t);
    
    for (auto it = P->get_F()->begin(); it != P->get_F()->end(); it++) {
        PDATransition t(*it, &empty_string, "", &q_e, &empty_string, CLOSE_DOWN);
        P_0->get_Delta()->add(&t);
    }

    for (auto it = P->get_Gamma()->begin(); it != P->get_Gamma()->end(); it++) {
        SymbolString pop;
        pop.append(*it);
        PDATransition t(&q_e, &pop, "", &q_e, &empty_string, CLOSE_DOWN);
        P_0->get_Delta()->add(&t);
    }

    PDATransition r(&q_e, &Z, "", &q_f, &empty_string, CLOSE_DOWN);
    P_0->get_Delta()->add(&r);

    return P_0;
}

PDACleanerResult clean_PDA(PDA *P, bool clean) {
    Sqs *sqs_map = new Sqs();
    PDA *P_0 = simplify_PDA(P);

    // As an over-approximation, U_1 initially contains all transitions in P_0
    PDATransitionSet *U_1 = P_0->get_Delta()->clone();
    
    // Build NFA of reachable configurations and only
    // keep unreachable transitions in U_1.
    NFA *N = build_NFA(P_0, U_1, sqs_map);

    // Creating alias to be consistent with the notation in the paper.
    PDA *P_1 = P_0;

    // Remove unreachable transitions.
    P_1->get_Delta()->remove_all(U_1);

    // As an over-approximation, U_2 initially contains all transitions in P_1.
    PDATransitionSet *U_2 = P_1->get_Delta()->clone();

    // Determine all non-finishing transitions. Only these will be contained in U_2
    // after the call returns.
    non_finishing(P_1, N, sqs_map, U_2);

    // Remove extension transitions from U_1 and U_2
    remove_extensions(U_1);
    remove_extensions(U_2);

    PDACleanerResult result = { U_1, U_2, nullptr };
    
    // Create a clean PDA, if requested
    if (clean) {
        PDA *P_clean = P->clone();
        P_clean->get_Delta()->remove_all(U_1);
        P_clean->get_Delta()->remove_all(U_2);
        
        // Remove unused states
        remove_unused_states(P_clean);

        result.P_clean = P_clean;
    }

    delete sqs_map;
    delete N;
    delete P_1;
    return result;
}
