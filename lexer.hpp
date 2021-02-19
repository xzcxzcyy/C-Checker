#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
private:
    std::ifstream inputStream;
    std::vector<Token> tokens;

public:
    Lexer(std::string fileName) {
        inputStream = std::ifstream(fileName);
        tokens.clear();
    }

    ~Lexer() {
        inputStream.close();
    }

    void analyze();
};

#endif