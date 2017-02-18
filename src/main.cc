#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <unistd.h>

#include "Parser.h"
#include "PDACleaner.h"

void formatted_output(PDACleanerResult& result, std::ostream& stream) {
    stream << "Unreachable transitions:\n";
    stream << "U_1 = " << result.U_1->to_string("\t", "\n") << "\n\n";
    
    stream << "Non-terminating transitions:\n";
    stream << "U_2 = " << result.U_2->to_string("\t", "\n") << "\n";

    if (result.P_clean != nullptr) {
        stream << "\nCleaned PDA:\n";
        stream << *result.P_clean << "\n";
    }
    
    stream.flush();
}

void print_result(std::string file, PDACleanerResult& result) {
    if (file != "") {
        std::ofstream file_stream(file);

        if (!file_stream.is_open()) {
            std::cerr << "Could not write to file: " << file << std::endl;
            formatted_output(result, std::cout);
        } else {
            formatted_output(result, file_stream);
            std::cout << "Output written to file: " << file << std::endl;
        }

        file_stream.close();
    } else {
        formatted_output(result, std::cout);
    }
}

PDA * read_input(std::string file, SymbolMode mode) {
    std::ifstream input(file);
    if (!input.is_open()) {
        std::cerr << "Could not open file: " << file << std::endl;
        exit(EXIT_FAILURE);
    }

    PDA *P = nullptr;
    
    try {
        P = parse_PDA(input, mode);
    } catch (std::exception& e) {
        std::cerr << "Input error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    input.close();
    return P;
}

std::string usage(char **argv) {
    std::stringstream ss;

    ss << "Usage: " << argv[0] << " [options] input_file [output_file]\n";
    ss << "Options:\n";
    ss << "  " << "-c" << "\t\t" << "print cleaned PDA\n";
    ss << "  " << "-i" << "\t\t" << "use multi-character identifiers as stack symbols\n";

    return ss.str();
}

void parse_arguments(int argc, char **argv, SymbolMode& mode, bool& clean, std::string& input, std::string& output) {
    int c = 0;

    while ((c = getopt(argc, argv, "ci")) != - 1) {
        switch (c) {
            case 'c':
                clean = true;
                break;
            case 'i':
                mode = IDENTIFIER;
                break;
            case '?':
                std::cerr << "Unknown option '" << c << "'" << std::endl;
                break;
        }
    }

    if (optind == argc) {
        std::cerr << usage(argv);
        exit(EXIT_FAILURE);
    }

    input = std::string(argv[optind]);

    if (optind + 1 < argc) {
        output = std::string(argv[optind + 1]);
    }
}

int main(int argc, char **argv) {
    SymbolMode mode = CHARACTER;
    bool clean = false;
    std::string input;
    std::string output;

    parse_arguments(argc, argv, mode, clean, input, output);

    PDA *P = read_input(input, mode);
    PDACleanerResult result = clean_PDA(P, clean);
    print_result(output, result);

    delete P;
    return EXIT_SUCCESS;
}
