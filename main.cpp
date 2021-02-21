#include <iostream>
#include "lexer.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test.c", 1);
    lexer.analyze();
    lexer.displayTokens(cout);
    return 0;
}
