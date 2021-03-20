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

    /**
     * Generate keywords map to tell a keyword from an identifier.
     */
    void inflateKeywords();

    /**
     * Panic mode: invoked on lexical error to consume the line on which the error exists.
     * @param inputStream
     * @param loopFlag : Whether automata continues to work after panic.
     * @param word : Word in automata.
     * @param state : State in automata.
     */
    void panic(std::ifstream &inputStream, bool &loopFlag, std::string &word, int &state);

    /**
     * Generate a token according to filename and line.
     * @param name
     * @param type
     * @return A generated token.
     */
    Token makeToken(std::string name, Token::TokenType type);

public:
    /**
     * Constructor.
     * @param fileName File which the lexer processes.
     */
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

    /**
     * Generate tokens from input file.
     */
    void analyze();

    /**
     * Display tokens.
     * @param out Output stream.
     * @param sortThem : Whether to sort tokens or not.
     */
    void displayTokens(std::ostream &out, bool sortThem);

    /**
     * Invoked on #include and on #define.
     * @param masterInput : Current input stream.
     * @param loopFlag
     * @param word
     * @param state
     */
    void preprocess(std::ifstream &masterInput, bool &loopFlag, std::string &word, int &state);
};

#endif