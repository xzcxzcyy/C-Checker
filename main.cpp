#include <iostream>
#include "lexer.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test/test.in");
    lexer.analyze();
    lexer.displayTokens(cout, true);
    lexer.displayTokens(cout, false);
    return 0;
}
