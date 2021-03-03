#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test/test.in");
    lexer.analyze();
    lexer.displayTokens(cout, true);
    // lexer.displayTokens(cout, false);

    auto parser = Parser(lexer.getTokens(), std::cout);
    auto root = parser.parse();
    return 0;
}
