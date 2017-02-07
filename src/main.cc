#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

#include "Parser.h"

#define USAGE_MSG "Usage: pdacleaner input_file [output_file]\n"

PDA * read_input(char *file) {
    std::ifstream input(file);
    if (!input.is_open()) {
        std::cerr << "Could not open file: " << file << std::endl;
        exit(EXIT_FAILURE);
    }

    PDA *P = nullptr;
    
    try {
        P = parse_PDA(input);
    } catch (std::exception& e) {
        std::cerr << "Input error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    input.close();
    return P;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << USAGE_MSG;
        return EXIT_FAILURE;
    }

    PDA *P = read_input(argv[1]);
    std::cout << *P;

    delete P;
    return EXIT_SUCCESS;
}
