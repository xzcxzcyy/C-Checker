#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Lexer {
private:
    std::string fileName;
    std::ifstream inputStream;
    std::vector<Token> tokens;
    std::unordered_map<std::string, Token::TokenType> keywords;
    int startLine;
    int errorCount;

    bool isUnderscore(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isEof(char c);
    bool isWhiteSpace(char c);
    void inflateKeywords();
    void panic(bool& loopFlag);

public:
    Lexer(std::string fileName, int startLine) {
        this->fileName = fileName;
        inputStream = std::ifstream(fileName);
        tokens.clear();
        inflateKeywords();
        this->startLine = startLine;
        errorCount = 0;
    }

    ~Lexer() {
        inputStream.close();
    }

    void analyze();
};

#endif