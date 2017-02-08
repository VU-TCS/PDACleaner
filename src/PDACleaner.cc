#include "PDACleaner.h"

PDACleanerResult clean_PDA(PDA *P) {
    PDATransitionSet U_1;
    PDATransitionSet U_2;
    PDACleanerResult result = { U_1, U_2, *P };
    return result;
}
