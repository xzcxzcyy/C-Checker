#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include "token.hpp"

class Lexer {
private:
    std::string fileName;
    std::vector<Token> tokens;
    std::unordered_map<std::string, Token::TokenType> keywords;
    int errorCount;
    int line = 1;

    static bool isUnderscore(char c);

    static bool isDigit(char c);

    static bool isLetter(char c);

    static bool isEof(char c);

    static bool isWhiteSpace(char c);

    static bool isHexadecimal(char c);

    void inflateKeywords();

    void panic(std::ifstream &inputStream, bool &loopFlag, std::string &word, int &state);

    Token makeToken(std::string name, Token::TokenType type);

public:
    explicit Lexer(std::string fileName) : fileName(std::move(fileName)) {
        tokens.clear();
        inflateKeywords();
        errorCount = 0;
    }

    std::vector<Token> &getTokens() {
        return tokens;
    }

    int getErrorCount() const {
        return errorCount;
    }

    void analyze();

    void displayTokens(std::ostream &out, bool sortThem);

    void preprocess(std::ifstream &masterInput, bool &loopFlag, std::string &word, int &state);
};

#endif