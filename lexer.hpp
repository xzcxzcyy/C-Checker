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
    int startLine;
    int errorCount;

    bool isUnderscore(char c);
    bool isDigit(char c);
    bool isLetter(char c);
    bool isEof(char c);
    bool isWhiteSpace(char c);
    bool isHexadecimal(char c);
    void inflateKeywords();
    void panic(std::ifstream& inputStream, bool& loopFlag, std::string& word, int& state);

public:
    Lexer(std::string fileName, int startLine) {
        this->fileName = fileName;
        // inputStream = std::ifstream(fileName);
        tokens.clear();
        inflateKeywords();
        this->startLine = startLine;
        errorCount = 0;
    }

    /*
    ~Lexer() {
        inputStream.close();
    }
    */

    std::vector<Token>& getTokens() {
        return tokens;
    }

    int getStartLine() {
        return startLine;
    }

    int getErrorCount() {
        return errorCount;
    }

    void analyze();
    void displayTokens(std::ostream& out);
};

#endif