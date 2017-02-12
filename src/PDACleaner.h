#ifndef PDACLEANER_H
#define PDACLEANER_H

#include "PDA.h"


/**
 * A structure that contains the ouput of clean_PDA().
 * U_1 contains all unreachable transitions.
 * U_2 contains all non-finishing transitions.
 * P_clean is the cleaned PDA.
 */
struct PDACleanerResult {
    PDATransitionSet *U_1;
    PDATransitionSet *U_2;
    PDA *P_clean;

    ~PDACleanerResult() {
        delete U_1;
        delete U_2;
        delete P_clean;
    }
};


/**
 * Detects all useless transitions in the specified PDA.
 * Returns a pointer to a heap-allocated PDACleanerResult object.
 * The caller is responsible for freeing the memory.
 *
 * @param P A pointer to the PDA to clean.
 * @return A pointer to a PDACleanerResult object allocated on the heap.
 */
PDACleanerResult clean_PDA(PDA *P);

#endif /* PDACLEANER_H */
