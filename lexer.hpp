#ifndef LEXER_HPP
#define LEXER_HPP
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "token.hpp"

class Lexer {
private:
    std::string fileName;
    std::ifstream inputStream;
    std::vector<Token> tokens;
    std::unordered_set<std::string> keywordSet;
    int startLine;
    int errorCount;

    bool isUnderscore(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isEof(char c);
    bool isWhiteSpace(char c);
    void panic(bool& loopFlag);

public:
    Lexer(std::string fileName, int startLine) {
        this->fileName = fileName;
        inputStream = std::ifstream(fileName);
        tokens.clear();
        keywordSet = { "int" }; // TODO: inflate keywordSet at here
        this->startLine = startLine;
        errorCount = 0;
    }

    ~Lexer() {
        inputStream.close();
    }

    void analyze();
};

#endif