#ifndef PDACLEANER_H
#define PDACLEANER_H

#include "PDA.h"

struct PDACleanerResult {
    PDATransitionSet U_1;
    PDATransitionSet U_2;
    PDA P_clean;
};

PDACleanerResult clean_PDA(PDA *P);

#endif /* PDACLEANER_H */
