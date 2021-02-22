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
    // std::ifstream inputStream;
    std::vector<Token> tokens;
    std::unordered_map<std::string, Token::TokenType> keywords;
    int errorCount;
    int line = 1;

    bool isUnderscore(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isEof(char c);
    bool isWhiteSpace(char c);
    bool isHexadecimal(char c);
    void inflateKeywords();
    void panic(std::ifstream& inputStream, bool& loopFlag, std::string& word, int& state);
    Token makeToken(std::string name, Token::TokenType type);

public:
    Lexer(std::string fileName) {
        this->fileName = fileName;
        tokens.clear();
        inflateKeywords();
        errorCount = 0;
    }

    std::vector<Token>& getTokens() {
        return tokens;
    }

    int getErrorCount() {
        return errorCount;
    }

    void analyze();
    void displayTokens(std::ostream& out, bool sortThem);
};

#endif