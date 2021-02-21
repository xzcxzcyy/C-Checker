#include <iostream>
#include "lexer.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test/test.in", 1);
    lexer.analyze();
    lexer.displayTokens(cout, true);
    return 0;
}
