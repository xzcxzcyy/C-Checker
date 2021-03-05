#include <iostream>
#include <string>
#include <vector>
#include "lexer.hpp"
#include "parser.hpp"

using namespace std;

/*
 * cchecker test.in -l
 * cchecker test.in -p
 * cchecker test.in -f test.out
 */

void printUsage();

void printLexicalErrors(const vector<Token> &tokens);

int main(int argc, char **argv) {
    string op;
    if (argc >= 3) {
        op = argv[2];
    }
    if (argc != 3 && argc != 4) {
        printUsage();
        return 1;
    }

    Lexer lexer = Lexer(argv[1]);
    lexer.analyze();
    vector<Token> tokens = lexer.getTokens();
    if (lexer.getErrorCount() > 0) {
        printLexicalErrors(tokens);
        return 0;
    }
    auto parser = Parser(tokens, cout);
    Node *tree = parser.parse();
    if (tree == nullptr) {
        return 0;
    }

    if (argc == 3 && op == "-l") {
        lexer.displayTokens(cout, true);
        delete tree;
        return 0;
    } else if (argc == 3 && op == "-p") {
        tree->display("");
        delete tree;
        return 0;
    } else if (argc == 4 && op == "-f") {
        // TODO: serialize AST
        delete tree;
        return 0;
    } else {
        printUsage();
        delete tree;
        return 1;
    }
}

void printUsage() {
    cout << "Usage: " << endl
         << "Show lexical analysis result: ./cchecker [in-file-name] -l" << endl
         << "Show abstract syntax tree: ./cchecker [in-file-name] -p" << endl
         << "Perform reformatting: ./cchecker [in-file-name] -f [out-file-name]" << endl;
}

void printLexicalErrors(const vector<Token> &tokens) {
    for (const auto &token : tokens) {
        if (token.type == Token::ERROR_TOKEN) {
            cout << token.fileName << ": line " << token.line << ": "
                 << token.name << endl;
        }
    }
}
