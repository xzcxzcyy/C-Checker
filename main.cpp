#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test/test.in");
    lexer.analyze();
    lexer.displayTokens(cout, false);

    auto parser = Parser(lexer.getTokens(), cout);
    auto root = parser.parse();
    if (root != nullptr) {
        // root->display("");
    }
    delete root;
    return 0;
}
