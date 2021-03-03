#include <iostream>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test/test3.in");
    lexer.analyze();
    lexer.displayTokens(cout, false);
    // lexer.displayTokens(cout, false);

    auto parser = Parser(lexer.getTokens(), cout);
    auto root = parser.parse();
    if (root != nullptr) {
        root->display(1);
    }
    delete root;
    return 0;
}
