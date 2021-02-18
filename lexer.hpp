#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <string>
#include <vector>
#include "token.hpp"

class Lexer {
private:
    std::ifstream inputStream;

public:
    Lexer(std::string fileName) {
        inputStream = std::ifstream(fileName);
    }

    ~Lexer() {
        inputStream.close();
    }

    std::vector<Token> analyze();
};

#endif