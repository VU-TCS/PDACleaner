#ifndef PARSER_H
#define PARSER_H

#include "PDA.h"

/**
 * Reads in a PDA from the provided stream.
 * Returns a pointer to a heap-allocated PDA object. The caller is responsible
 * for freeing the memory. Throws an exception if the input is invalid.
 *
 * @param input The input stream to read the PDA from.
 * @return A pointer to a PDA object allocated on the heap.
 */
PDA * parse_PDA(std::ifstream& input);

#endif /* PARSER_H */
