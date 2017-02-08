#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdexcept>

#include "Parser.h"
#include "PDACleaner.h"

#define USAGE_MSG(bin) "Usage: " + bin + " input_file [output_file]\n"

void formatted_output(PDACleanerResult& result, std::ostream& stream) {    
    stream << "U_1 = " << result.U_1.to_string("\t", "\n") << std::endl;
    stream << "U_2 = " << result.U_2.to_string("\t", "\n") << std::endl;
    stream << result.P_clean << std::endl;
    stream.flush();
}

void print_result(int argc, char **argv, PDACleanerResult& result) {
    if (argc == 3) {
        std::ofstream file_stream(argv[2]);

        if (!file_stream.is_open()) {
            std::cerr << "Could not write to file: " << argv[2] << std::endl;
            formatted_output(result, std::cout);
        } else {
            formatted_output(result, file_stream);
        }

        file_stream.close();
    } else {
        formatted_output(result, std::cout);
    }
}

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
        std::cerr << USAGE_MSG(std::string(argv[0]));
        return EXIT_FAILURE;
    }

    PDA *P = read_input(argv[1]);
    PDACleanerResult result = clean_PDA(P);
    print_result(argc, argv, result);
    
    delete P;
    return EXIT_SUCCESS;
}
