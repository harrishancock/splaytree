/*
 * California State University East Bay
 * CS4110 - Compiler Design
 * Author: Harris Hancock (hhancock 'at' horizon)
 *
 * Symbol Table Assignment (8 October 2013)
 *
 * main.cpp
 *
 * The following program drives a symbol_table_scope_manager object
 * (documented later) with identifiers from an input file. The input file
 * format is a whitespace-delimited sequence of open-scope, close-scope, and
 * identifier lexemes. An open-scope lexeme is "{". A close-scope lexeme is
 * "}". An identifier lexeme is any string of printable, non-whitespace,
 * non-curly-brace characters. No syntax checking on the number or order of
 * open- and close-scope lexemes is done; however, the normal rules should be
 * observed or this test program is not guaranteed to work.
 *
 * Test files can be specified as an argument on the command line, or entered
 * on standard input. Usage of this program is therefore one of the two:
 *
 * $ ./main < testfile
 * $ ./main testfile
 *
 * Both forms should accomplish the same task.
 *
 * This program was tested with gcc 4.7.3 and clang 3.2 on an Ubuntu 13.04
 * GNU/Linux system. It uses C++11, so compile like so:
 *
 * $ g++ -std=c++11 -o main main.cpp
 */

#include "symbol_table.hpp"

#include <cassert>

#include <iostream>
#include <fstream>

const std::string open_scope_lexeme { "{" };
const std::string close_scope_lexeme { "}" };

/* Exercise a symbol table with lexemes from the given input stream. */
void test_symtab (symbol_table_scope_manager& symtab, std::istream& input);

//////////////////////////////////////////////////////////////////////////////

int main (int argc, char** argv) try {
    symbol_table_scope_manager symtab;

    if (argc > 1) {
        /* Use the file whose name was passed on the command line. */
        std::ifstream input { argv[1] };
        test_symtab(symtab, input);
    }
    else {
        /* Use stdin. */
        test_symtab(symtab, std::cin);
    }

    symtab.display();
    return 0;
}
catch (std::exception& exc) {
    std::cout << "test failed: " << exc.what() << '\n';
}
catch (...) {
    std::cout << "unknown exception\n";
}

//////////////////////////////////////////////////////////////////////////////

void test_symtab (symbol_table_scope_manager& symtab, std::istream& input) {
    /* Note that std::istream's extraction operator automatically skips
     * whitespace. std::string handles all character array allocation,
     * so our identifiers will be stored without truncation. More
     * relevantly, the associative data structure underlying the symbol
     * table also uses std::string as part of its key type, so each
     * identifier has its own dynamically-allocated section of memory.
     * lexeme simply serves as an input buffer. */
    std::string lexeme;
    while (input >> lexeme) {
        if (!lexeme.length()) {
            continue;
        }

        if (open_scope_lexeme == lexeme) {
            symtab.open_scope();
        }
        else if (close_scope_lexeme == lexeme) {
            symtab.close_scope();
        }
        else {
            /* symbol_table_scope_manager::insert returns a std::pair of an
             * iterator to the element in question, and a boolean reflecting
             * whether or not insertion actually took place (i.e., if an
             * identifier matching lexeme already exists in this scope, this
             * success flag will be false). I included an assertion below
             * simply to show that this boolean works as intended. */
            symbol_table_scope_manager::iterator it;
            bool success;
            std::tie(it, success) = symtab.insert(lexeme);

            /* The iterator part of the std::pair "points" to the element,
             * which is in turn represented by a std::pair of the key and the
             * record. */
            auto& record = it->second;

            /* Either we successfully inserted, or this identifier appeared in
             * this scope already. */
            assert(success || record.reference_count);

            /* Record a reference (in the abstract sense, not the C++ sense)
             * to this identifier. Mostly just for funsies. */
            ++record.reference_count;
        }
    }
}
