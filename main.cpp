#include <iostream>
#include "lexer.hpp"

using namespace std;

int main() {
    auto lexer = Lexer("test.c", 1);
    lexer.analyze();
    auto tokens = lexer.getTokens();
    
    for (const auto& token : tokens) {
        cout << token.name << endl;
    }

    return 0;
}
